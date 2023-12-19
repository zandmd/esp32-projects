#include <assert.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdint.h>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/color/color.hpp>
#include <zandmd/color/color_cast.hpp>
#include <zandmd/drivers/button.hpp>

#define TAG "main"

using namespace zandmd::bsp;
using namespace zandmd::color;
using namespace zandmd::drivers;

extern "C" void app_main() {
    peripherals::btn.enable();
    peripherals::btn.callback = [](button::event ev, TickType_t time) {
        switch (ev) {
            case button::press:
                ESP_LOGD(TAG, "button: press");
                break;
            case button::release:
                ESP_LOGD(TAG, "button: release");
                break;
            case button::click:
                ESP_LOGI(TAG, "button: click");
                break;
            case button::hold:
                ESP_LOGI(TAG, "button: hold");
                break;
            case button::double_click:
                ESP_LOGI(TAG, "button: double_click");
                break;
            case button::triple_click:
                ESP_LOGI(TAG, "button: triple_click");
                break;
            default:
                ESP_LOGW(TAG, "button: state %d", ev);
                break;
        }
    };
    assert(xTaskCreate([](void *) {
        color<hsv, uint8_t> color(0, 255, 15);
        while (true) {
            ++color.hue();
            ws2811::color_grb converted = color_cast<ws2811::color_grb::format, ws2811::color_grb::rep>(color);
            peripherals::status_led.start(&converted, 1);
            peripherals::status_led.wait();
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }, "led task", 0x1000, nullptr, 2, nullptr) == pdPASS);
}
