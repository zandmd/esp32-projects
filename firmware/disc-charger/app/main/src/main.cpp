#include <driver/rtc_io.h>
#include <esp_err.h>
#include <esp_sleep.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/analog.hpp>
#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/button.hpp>
#include <zandmd/drivers/ws2811.hpp>

using namespace zandmd::bsp;
using namespace zandmd::drivers;

static void shutdown();

extern "C" void app_main() {
    ESP_ERROR_CHECK(rtc_gpio_deinit(gpio::btn));
    ESP_ERROR_CHECK(rtc_gpio_deinit(gpio::adc_5v));
    while (true) {
        if (peripherals::adc.usb()) {
            ws2811::color_grb red(63, 0, 0);
            peripherals::status_led.start(&red, 1);
            peripherals::status_led.wait();
            while (peripherals::adc.battery() != analog::charged) {
                if (!peripherals::adc.usb()) {
                    shutdown();
                }
                vTaskDelay(pdMS_TO_TICKS(100));
            }
            ws2811::color_grb green(0, 63, 0);
            peripherals::status_led.start(&green, 1);
            peripherals::status_led.wait();
            while (peripherals::adc.usb()) {
                vTaskDelay(pdMS_TO_TICKS(100));
            }
            shutdown();
        } else {
            peripherals::btn.enable();
            peripherals::btn.callback = [](button::event ev, TickType_t time) {
                shutdown();
            };
            peripherals::uv.on(1.f);
            while (!peripherals::adc.usb()) {
                vTaskDelay(pdMS_TO_TICKS(100));
            }
            peripherals::uv.off();
        }
    }
}

static void shutdown() {
    ws2811::color_grb zero(0, 0, 0);
    peripherals::status_led.start(&zero, 1);
    peripherals::status_led.wait();
    peripherals::uv.off();
    vTaskDelay(pdMS_TO_TICKS(1000));
    ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(gpio::btn, false));
    ESP_ERROR_CHECK(esp_sleep_enable_ext1_wakeup(1uLL << gpio::adc_5v, ESP_EXT1_WAKEUP_ANY_HIGH));
    ESP_ERROR_CHECK(rtc_gpio_pulldown_dis(gpio::btn));
    ESP_ERROR_CHECK(rtc_gpio_pullup_en(gpio::btn));
    ESP_ERROR_CHECK(rtc_gpio_pulldown_dis(gpio::adc_5v));
    ESP_ERROR_CHECK(rtc_gpio_pullup_dis(gpio::adc_5v));
    esp_deep_sleep_start();
    assert(false);
}
