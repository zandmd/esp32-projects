#include <limits>
#include <driver/gpio.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <zandmd/drivers/button.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

using namespace std;
using namespace zandmd::drivers;
using namespace zandmd::peripheral_alloc;

button::button(generic_gpio gpio) noexcept : gpio(gpio), last_up(0) {
    ESP_ERROR_CHECK(gpio_reset_pin(gpio));
    ESP_ERROR_CHECK(gpio_set_direction(gpio, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(gpio, GPIO_PULLUP_ONLY));
    ESP_ERROR_CHECK(gpio_set_intr_type(gpio, GPIO_INTR_ANYEDGE));
    semaphore = xSemaphoreCreateCountingStatic(numeric_limits<UBaseType_t>::max(), 0, &semaphore_memory);
}

button::~button() noexcept {
    vSemaphoreDelete(semaphore);
    ESP_ERROR_CHECK(gpio_reset_pin(gpio));
}

void button::enable() noexcept {
    add(gpio);
    ESP_ERROR_CHECK(gpio_intr_enable(gpio));
}

void button::disable() noexcept {
    ESP_ERROR_CHECK(gpio_intr_disable(gpio));
    remove(gpio);
}

bool button::wait(TickType_t timeout) const noexcept {
    return xSemaphoreTake(semaphore, timeout) == pdTRUE;
}

void button::isr() noexcept {
    TickType_t now = xTaskGetTickCountFromISR();
    bool is_button_up = gpio_get_level(gpio);
    if (is_button_up) {
        last_up = now;
    } else {
        if (now - last_up >= pdMS_TO_TICKS(100)) {
            xSemaphoreGiveFromISR(semaphore, nullptr);
        }
    }
}
