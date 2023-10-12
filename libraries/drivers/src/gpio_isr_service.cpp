#include <driver/gpio.h>
#include <esp_err.h>
#include <zandmd/drivers/gpio_isr_service.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

using namespace zandmd::drivers;
using namespace zandmd::peripheral_alloc;

unsigned gpio_isr_service::count = 0;

void gpio_isr_service::add(generic_gpio gpio) noexcept {
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
}

void gpio_isr_service::call_isr(void *context) noexcept {
    static_cast<gpio_isr_service *>(context)->isr();
}
