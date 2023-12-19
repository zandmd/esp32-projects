#include <driver/gpio.h>
#include <driver/ledc.h>
#include <esp_err.h>
#include <stdint.h>
#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/leds.hpp>

using namespace zandmd::bsp;

const uint32_t leds::frequency;

leds::leds() noexcept {
    const gpio_config_t dio_cfg = {
        .pin_bit_mask = 1uLL << gpio::led_ctrl,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&dio_cfg));
    const ledc_timer_config_t timer_cfg = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = frequency,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_cfg));
    const ledc_channel_config_t pwm_cfg = {
        .gpio_num = gpio::led_dim,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
        .flags = {
            .output_invert = false
        }
    };
    ESP_ERROR_CHECK(ledc_channel_config(&pwm_cfg));
}

void leds::off() noexcept {
    ESP_ERROR_CHECK(gpio_set_level(gpio::led_ctrl, false));
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
}

void leds::on(float duty_cycle) noexcept {
    uint32_t duty = static_cast<uint32_t>((1.f - duty_cycle) * 0x400);
    if (duty > 0x80000000) {
        duty = 0;
    } else if (duty > 0x400) {
        duty = 0x400;
    }
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
    ESP_ERROR_CHECK(gpio_set_level(gpio::led_ctrl, true));
}
