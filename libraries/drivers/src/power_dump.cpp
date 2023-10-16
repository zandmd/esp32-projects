#include <assert.h>
#include <chrono>
#include <driver/gpio.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <soc/rtc_cntl_reg.h>
#include <soc/soc.h>
#include <zandmd/drivers/power_dump.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

using namespace std::chrono;
using namespace zandmd::drivers;
using namespace zandmd::peripheral_alloc;

power_dump::power_dump(generic_gpio gpio, milliseconds default_duration) : gpio(gpio), default_duration(default_duration) {
    semaphore = xSemaphoreCreateBinaryStatic(&semaphore_mem);
    assert(semaphore != nullptr);
    gpio_config_t cfg = {
        .pin_bit_mask = 1u << gpio,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_set_level(gpio, false));
    ESP_ERROR_CHECK(gpio_config(&cfg));
    ESP_ERROR_CHECK(gpio_set_level(gpio, false));
}

power_dump::~power_dump() noexcept {
    vSemaphoreDelete(semaphore);
}

bool power_dump::arm() noexcept {
    return xSemaphoreGive(semaphore) == pdTRUE;
}

bool power_dump::disarm() noexcept {
    return xSemaphoreTake(semaphore, 0) == pdTRUE;
}

bool power_dump::is_armed() const noexcept {
    return uxSemaphoreGetCount(semaphore) > 0;
}

bool power_dump::dump(milliseconds duration) noexcept {
    if (!duration.count()) {
        duration = default_duration;
    }
    if (disarm()) {
        WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
        ESP_ERROR_CHECK(gpio_set_level(gpio, true));
        vTaskDelay(pdMS_TO_TICKS(duration.count()));
        ESP_ERROR_CHECK(gpio_set_level(gpio, false));
        WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1);
        return true;
    }
    return false;
}
