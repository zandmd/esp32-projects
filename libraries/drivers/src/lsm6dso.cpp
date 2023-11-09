#include <algorithm>
#include <assert.h>
#include <stdint.h>
#include <zandmd/bus/spidev.hpp>
#include <zandmd/drivers/gpio_isr_service.hpp>
#include <zandmd/drivers/lsm6dso.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

using namespace std;
using namespace zandmd::bus;
using namespace zandmd::drivers;
using namespace zandmd::peripheral_alloc;

// Write registers
static constexpr uint8_t FIFO_CTRL1 = 0x07;
static constexpr uint8_t INT1_CTRL = 0x0D;
static constexpr uint8_t CTRL1_XL = 0x10;
static constexpr uint8_t CTRL3_C = 0x12;

// Read registers
static constexpr uint8_t WHO_AM_I = 0x8F;
static constexpr uint8_t FIFO_STATUS1 = 0xBA;
static constexpr uint8_t FIFO_DATA_OUT_TAG = 0xF8;

lsm6dso::lsm6dso(spidev &spi, generic_gpio int1) noexcept : spi(spi), int1(int1) {
}

void lsm6dso::enable(UBaseType_t priority, odr accel_odr, accel_fs accel_fs, odr gyro_odr, gyro_fs gyro_fs, sample *samples, size_t samples_count) noexcept {
    assert(samples != nullptr);
    uint8_t who_am_i;
    spi << WHO_AM_I >> who_am_i;
    assert(who_am_i == 0x6C);
    spi << CTRL3_C << '\x01'; // SW_RESET
    static const uint8_t accel_fs_table[] = {
        0, 2, 3, 1
    };
    uint8_t ctrl[] = {
        static_cast<uint8_t>((accel_odr << 4) | (accel_fs_table[accel_fs & 3] << 2)),
        static_cast<uint8_t>((gyro_odr << 4) | (gyro_fs << 2)),
        0x44, // BDU | IF_INC
    };
    spi << CTRL1_XL << ctrl;
    samples_count = min(samples_count, 0x1FFu);
    uint8_t fifo[] = {
        static_cast<uint8_t>(samples_count),
        static_cast<uint8_t>(samples_count >> 8),
        static_cast<uint8_t>((gyro_odr << 4) | accel_odr),
        0x06 // Continuous mode
    };
    spi << FIFO_CTRL1 << fifo;
    spi << INT1_CTRL << '\x08'; // INT1_FIFO_TH
    this->samples = samples;
    this->samples_count = samples_count;
    task = xTaskCreateStatic(&lsm6dso::task_func, "lsm6dso", sizeof(stack), this, priority, stack, &task_mem);
    assert(task != nullptr);
}

void lsm6dso::disable() noexcept {
    samples = nullptr;
    assert(xTaskNotifyGive(task) == pdPASS);
    uint8_t ctrl[] = {
        0, 0
    };
    spi << CTRL1_XL << ctrl;
}

void lsm6dso::isr() noexcept {
    vTaskNotifyGiveFromISR(task, nullptr);
}

void lsm6dso::task_func(void *context) noexcept {
    lsm6dso &lsm = *static_cast<lsm6dso *>(context);
    while (true) {
        sample *samples = lsm.samples;
        if (!samples) {
            vTaskDelete(nullptr);
            return;
        }
        uint8_t status[2];
        lsm.spi << FIFO_STATUS1 >> status;
        assert((status[1] & 0x68) == 0); // FIFO_OVR_IA, FIFO_FULL_IA, FIFO_OVR_LATCHED
        size_t count = static_cast<size_t>(status[0]) | (static_cast<size_t>(status[1] & 0x03) << 8);
        count = min(count, lsm.samples_count);
        for (size_t i = 0; i < count; ++i) {
            lsm.spi << FIFO_DATA_OUT_TAG >> lsm.samples[i];
        }
        if (lsm.callback && count) {
            lsm.callback(lsm.samples, count);
        }
        ulTaskNotifyTake(true, pdMS_TO_TICKS(50)); // TODO: Fix interrupts
    }
}
