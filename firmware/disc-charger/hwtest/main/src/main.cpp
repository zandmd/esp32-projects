#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/button.hpp>

#define TAG "main"

using namespace zandmd::bsp;
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
}
