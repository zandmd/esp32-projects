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

bool power_dump::dump_all(power_dump **begin, power_dump **end, milliseconds duration) noexcept {
    if (begin == end) {
        return true;
    }
    if (!duration.count()) {
        duration = (*begin)->default_duration;
        for (power_dump **it = begin + 1; it != end; ++it) {
            if (duration != (*it)->default_duration) {
                return false;
            }
        }
    }
    bool valid = true;
    for (power_dump **it = begin; it != end; ++it) {
        if (!(*it)->disarm()) {
            valid = false;
        }
    }
    if (!valid) {
        return false;
    }
    UBaseType_t old_priority = uxTaskPriorityGet(nullptr);
    vTaskPrioritySet(nullptr, configMAX_PRIORITIES - 1);
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    esp_err_t err = ESP_OK;
    for (power_dump **it = begin; it != end; ++it) {
        err = gpio_set_level((*it)->gpio, true);
        if (err != ESP_OK) {
            break;
        }
    }
    if (err == ESP_OK) {
        vTaskDelay(pdMS_TO_TICKS(duration.count()));
    }
    for (power_dump **it = begin; it != end; ++it) {
        esp_err_t new_err = gpio_set_level((*it)->gpio, false);
        if (new_err != ESP_OK) {
            err = new_err;
        }
    }
    vTaskDelay(1);
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1);
    ESP_ERROR_CHECK(err);
    vTaskPrioritySet(nullptr, old_priority);
    return true;
}
