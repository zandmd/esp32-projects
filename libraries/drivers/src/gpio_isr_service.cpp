#include <driver/gpio.h>
#include <esp_err.h>
#include <hal/gpio_types.h>
#include <zandmd/drivers/gpio_isr_service.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

using namespace zandmd::drivers;
using namespace zandmd::peripheral_alloc;

unsigned gpio_isr_service::count = 0;

void gpio_isr_service::add(generic_gpio gpio, gpio_int_type_t type) noexcept {
    gpio_config_t cfg = {
        .pin_bit_mask = 1u << gpio,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = type
    };
    switch (type) {
        case GPIO_INTR_POSEDGE:
        case GPIO_INTR_HIGH_LEVEL:
            cfg.pull_down_en = GPIO_PULLDOWN_ENABLE;
            break;
        case GPIO_INTR_NEGEDGE:
        case GPIO_INTR_LOW_LEVEL:
            cfg.pull_up_en = GPIO_PULLUP_ENABLE;
            break;
        default:
            break;
    }
    ESP_ERROR_CHECK(gpio_config(&cfg));
    if (++count == 1) {
        ESP_ERROR_CHECK(gpio_install_isr_service(0));
    }
    ESP_ERROR_CHECK(gpio_isr_handler_add(gpio, &gpio_isr_service::call_isr, this));
}

void gpio_isr_service::remove(generic_gpio gpio) noexcept {
    ESP_ERROR_CHECK(gpio_isr_handler_remove(gpio));
    if (--count == 0) {
        gpio_uninstall_isr_service();
    }
    ESP_ERROR_CHECK(gpio_reset_pin(gpio));
}

void gpio_isr_service::call_isr(void *context) noexcept {
    static_cast<gpio_isr_service *>(context)->isr();
}
