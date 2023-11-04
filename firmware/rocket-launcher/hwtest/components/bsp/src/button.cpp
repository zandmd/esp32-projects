#include <driver/gpio.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/tasks.hpp>

#include <zandmd/bsp/button.hpp>

using namespace zandmd::bsp;

button::button() noexcept {
    xTaskCreate(&button::poll_buttons, "poll button", 0x1000, this, tasks::buttontask, NULL);
}

button::~button() noexcept {

}

bool button::get_button_state(int buttonnum) noexcept {
    return buttonval[buttonnum];
}

void button::poll_buttons(void * context) noexcept {
    gpio_config_t cfgcol = {
        .pin_bit_mask = (1uLL << gpio::btn_c0) | (1uLL << gpio::btn_c1),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

        gpio_config_t cfgrow = {
        .pin_bit_mask = (1uLL << gpio::btn_r0) | (1uLL << gpio::btn_r1) | (1uLL << gpio::btn_r2),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_set_level(gpio::btn_r0,false);
    gpio_set_level(gpio::btn_r1,false);
    gpio_set_level(gpio::btn_r2,false);
    gpio_config(&cfgcol);
    gpio_config(&cfgrow);

    button* btn = (button*)context;

    while (1) {

        gpio_set_level(gpio::btn_r0,true);
        gpio_set_level(gpio::btn_r1,false);
        gpio_set_level(gpio::btn_r2,false);
        btn->buttonval[0] = gpio_get_level(gpio::btn_c0);
        btn->buttonval[1] = gpio_get_level(gpio::btn_c1);

        gpio_set_level(gpio::btn_r0,false);
        gpio_set_level(gpio::btn_r1,true);
        gpio_set_level(gpio::btn_r2,false);
        btn->buttonval[2] = gpio_get_level(gpio::btn_c0);
        btn->buttonval[3] = gpio_get_level(gpio::btn_c1);

        gpio_set_level(gpio::btn_r0,false);
        gpio_set_level(gpio::btn_r1,false);
        gpio_set_level(gpio::btn_r2,true);
        btn->buttonval[4] = gpio_get_level(gpio::btn_c0);
        btn->buttonval[5] = gpio_get_level(gpio::btn_c1);
    
    }
}
