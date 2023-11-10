#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <rom/crc.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <zandmd/bus/spidev.hpp>
#include <zandmd/drivers/lora.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

#define TAG "lora"

using namespace zandmd::bus;
using namespace zandmd::drivers;
using namespace zandmd::peripheral_alloc;

// Write registers
static constexpr uint8_t WRITE_FIFO = 0x80;

static constexpr uint8_t OP_MODE = 0x81;
static constexpr uint8_t OP_MODE_SLEEP = 0x80; // Sleep | HF | FSK | LoRa
static constexpr uint8_t OP_MODE_STANDBY = 0x81; // Standby | HF | FSK | LoRa
static constexpr uint8_t OP_MODE_TX = 0x83; // Transmit | HF | FSK | LoRa
static constexpr uint8_t OP_MODE_RECV_CONTINUOUS = 0x85; // Receive continuous | HF | FSK | LoRa

static constexpr uint8_t FR_MSB = 0x86;
static constexpr uint8_t FR_MSB_915 = 0xE4;
static constexpr uint8_t FR_MID_915 = 0xC0;
static constexpr uint8_t FR_LSB_915 = 0x00;

static constexpr uint8_t PA_CONFIG = 0x89;
static constexpr uint8_t PA_CONFIG_P17_DBM = 0x8F; // PA_BOOST

static constexpr uint8_t OCP = 0x8B;
static constexpr uint8_t OCP_100_MA = 0x2B;

static constexpr uint8_t LNA = 0x8C;
static constexpr uint8_t LNA_MAX = 0x23; // Boost on | maximum gain

static constexpr uint8_t FIFO_ADDR_PTR = 0x8D;

static constexpr uint8_t FIFO_TX_BASE_ADDR = 0x8E;

static constexpr uint8_t FIFO_RX_BASE_ADDR = 0x8F;

static constexpr uint8_t WRITE_IRQ_FLAGS = 0x92;

static constexpr uint8_t PAYLOAD_LENGTH = 0xA2;

static constexpr uint8_t MODEM_CONFIG_3 = 0xA6;
static constexpr uint8_t MODEM_CONFIG_3_DEF = 0x04; // Manual LNA gain | mobile node

static constexpr uint8_t DIO_MAPPING_1 = 0xC0; // bits 7-6 are for dio0
static constexpr uint8_t DIO_MAPPING_1_NONE = 0xFF;
static constexpr uint8_t DIO_MAPPING_1_RX_DONE_IO0 = 0x3F;
static constexpr uint8_t DIO_MAPPING_1_TX_DONE_IO0 = 0x7F;

static constexpr uint8_t PA_DAC = 0xCD;
static constexpr uint8_t PA_DAC_NORMAL = 0x84; // ~(+20 dBm)

// Read registers
static constexpr uint8_t READ_FIFO = 0x00;

static constexpr uint8_t FIFO_RX_CURRENT_ADDR = 0x10;

static constexpr uint8_t READ_IRQ_FLAGS = 0x12;
static constexpr uint8_t IRQ_FLAGS_TX_DONE = 0x08;
static constexpr uint8_t IRQ_FLAGS_CRC_ERR = 0x20;
static constexpr uint8_t IRQ_FLAGS_RX_DONE = 0x40;

static constexpr uint8_t RX_NB_BYTES = 0x13;

static constexpr uint8_t PKT_SNR_VALUE = 0x19;

static constexpr uint8_t PKT_RSSI_VALUE = 0x1A;

static constexpr uint8_t VERSION = 0x42;

lora::lora(spidev &spi, generic_gpio reset, generic_gpio irq, const char *ssid) : spi(spi), reset(reset), irq(irq), task(nullptr), enabled(false) {
    sem = xSemaphoreCreateBinaryStatic(&sem_mem);
    assert(sem != nullptr);
    this->ssid = crc32_le(0, reinterpret_cast<const uint8_t *>(ssid), strlen(ssid));
}

lora::~lora() noexcept {
    assert(xSemaphoreTake(sem, 0) == pdTRUE);
    vSemaphoreDelete(sem);
}

void lora::enable() noexcept {
    assert(!enabled);

    gpio_config_t reset_cfg = {
        .pin_bit_mask = 1u << reset,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_set_level(reset, false));
    ESP_ERROR_CHECK(gpio_config(&reset_cfg));
    ESP_ERROR_CHECK(gpio_set_level(reset, false));
    vTaskDelay(pdMS_TO_TICKS(10));
    ESP_ERROR_CHECK(gpio_set_level(reset, true));
    vTaskDelay(pdMS_TO_TICKS(20));

    uint8_t version;
    spi << VERSION >> version;
    assert(version == 0x12);

    spi << OP_MODE << OP_MODE_SLEEP;
    spi << FR_MSB << FR_MSB_915 << FR_MID_915 << FR_LSB_915;
    spi << PA_CONFIG << PA_CONFIG_P17_DBM;
    spi << OCP << OCP_100_MA;
    spi << LNA << LNA_MAX;
    spi << FIFO_TX_BASE_ADDR << static_cast<uint8_t>(0);
    spi << FIFO_RX_BASE_ADDR << static_cast<uint8_t>(0);
    spi << MODEM_CONFIG_3 << MODEM_CONFIG_3_DEF;
    spi << DIO_MAPPING_1 << DIO_MAPPING_1_NONE;
    spi << PA_DAC << PA_DAC_NORMAL;
    spi << OP_MODE << OP_MODE_STANDBY;

    add(irq, GPIO_INTR_POSEDGE);
    enabled = true;
    assert(xSemaphoreGive(sem) == pdTRUE);
}

void lora::disable() noexcept {
    assert(enabled);
    enabled = false;
    remove(irq);
    cancel();
    assert(xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE);
    spi << OP_MODE << OP_MODE_SLEEP;
}

void lora::get_debug(int &rssi, float &snr) noexcept {
    assert(xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE);
    assert(enabled);

    uint8_t rssi_raw;
    spi << PKT_RSSI_VALUE >> rssi_raw;
    rssi = static_cast<int>(rssi_raw) - 157;

    int8_t snr_raw;
    spi << PKT_SNR_VALUE >> snr_raw;
    snr = static_cast<float>(snr_raw) / 4.f;

    assert(xSemaphoreGive(sem) == pdTRUE);
}

void lora::cancel() noexcept {
    TaskHandle_t t = task;
    if (t) {
        canceled = true;
        assert(xTaskNotifyGive(t) == pdPASS);
    }
}

uint8_t lora::send_pre(size_t size) noexcept {
    assert(size <= MAX_PACKET_LENGTH);
    assert(xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE);
    assert(enabled);

    spi << FIFO_ADDR_PTR << 0;
    spi << WRITE_FIFO << static_cast<uint32_t>(ssid);
    spi << PAYLOAD_LENGTH << static_cast<uint8_t>(sizeof(uint32_t) + size);
    return WRITE_FIFO;
}

void lora::send_post() noexcept {
    task = xTaskGetCurrentTaskHandle();
    spi << DIO_MAPPING_1 << DIO_MAPPING_1_TX_DONE_IO0;
    spi << OP_MODE << OP_MODE_TX;

    while (true) {
        ulTaskNotifyTake(true, portMAX_DELAY);
        if (!enabled) {
            break;
        }

        uint8_t irqs;
        spi << READ_IRQ_FLAGS >> irqs;
        spi << WRITE_IRQ_FLAGS << irqs;
        if ((irqs & IRQ_FLAGS_TX_DONE) != IRQ_FLAGS_TX_DONE) {
            continue;
        }

        break;
    }

    spi << OP_MODE << OP_MODE_STANDBY;
    spi << DIO_MAPPING_1 << DIO_MAPPING_1_NONE;
    task = nullptr;
    assert(xSemaphoreGive(sem) == pdTRUE);
}

uint8_t lora::recv_pre(size_t size) noexcept {
    assert(size <= MAX_PACKET_LENGTH);
    assert(xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE);
    assert(enabled);
    canceled = false;

    task = xTaskGetCurrentTaskHandle();
    spi << DIO_MAPPING_1 << DIO_MAPPING_1_RX_DONE_IO0;
    spi << OP_MODE << OP_MODE_RECV_CONTINUOUS;

    while (true) {
        ulTaskNotifyTake(true, portMAX_DELAY);
        if (!enabled || canceled) {
            return 0xFF;
        }

        uint8_t irqs;
        spi << READ_IRQ_FLAGS >> irqs;
        spi << WRITE_IRQ_FLAGS << irqs;
        if ((irqs & (IRQ_FLAGS_CRC_ERR | IRQ_FLAGS_RX_DONE)) != IRQ_FLAGS_RX_DONE) {
            ESP_LOGW(TAG, "Bad interrupt flags for receive: %02X", irqs);
            continue;
        }

        uint8_t length;
        spi << RX_NB_BYTES >> length;
        if (length != size + sizeof(uint32_t)) {
            ESP_LOGW(TAG, "Got bad length %d (expected %d)", length, size + sizeof(uint32_t));
            continue;
        }

        uint8_t addr;
        spi << FIFO_RX_CURRENT_ADDR >> addr;
        spi << FIFO_ADDR_PTR << addr;

        uint32_t recv_ssid;
        spi << READ_FIFO >> recv_ssid;
        if (recv_ssid != ssid) {
            ESP_LOGW(TAG, "Got invalid SSID %08lX (expected %08lX)", recv_ssid, ssid);
            continue;
        }

        return READ_FIFO;
    }
}

void lora::recv_post() noexcept {
    spi << OP_MODE << OP_MODE_STANDBY;
    spi << DIO_MAPPING_1 << DIO_MAPPING_1_NONE;
    task = nullptr;
    assert(xSemaphoreGive(sem) == pdTRUE);
}

void lora::isr() noexcept {
    if (task) {
        vTaskNotifyGiveFromISR(task, nullptr);
    }
}
