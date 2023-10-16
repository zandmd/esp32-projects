#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/bus.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/color/color.hpp>
#include <zandmd/color/color_cast.hpp>
#include <zandmd/color/hsv.hpp>
#include <zandmd/drivers/button.hpp>
#include <zandmd/drivers/lsm6dso.hpp>
#include <zandmd/drivers/ws2811.hpp>

#define TAG "main"

using namespace zandmd::bsp;
using namespace zandmd::color;
using namespace zandmd::drivers;

extern "C" void app_main() {
    peripherals::button.enable();
    peripherals::button.callback = [](button::event ev, TickType_t time) {
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
            ws2811::color converted = color_cast<ws2811::color::format, ws2811::color::rep>(color);
            peripherals::led.start(&converted, 1);
            peripherals::led.wait();
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }, "led task", 0x1000, nullptr, 2, nullptr) == pdPASS);
    uint8_t whoami;
    bus::imu << '\x8F' >> whoami;
    ESP_LOGI(TAG, "IMU: 0x8F 0x%02X", whoami);
    static lsm6dso::sample samples[104];
    lsm6dso::accel_fs fs = lsm6dso::accel_4_g;
    peripherals::imu.callback = [fs](const lsm6dso::sample *samples, size_t count) {
        for (size_t i = 0; i < count; ++i) {
            if (samples[i].tag.sensor() == lsm6dso::sample::tag_type::accel_nc) {
                ESP_LOGI(TAG, "IMU: %d samples: <%.3f, %.3f, %.3f> g", count, samples[i].x.accel(fs), samples[i].y.accel(fs), samples[i].z.accel(fs));
                return;
            }
        }
        ESP_LOGI(TAG, "IMU: %d samples", count);
    };
    peripherals::imu.enable(4, lsm6dso::odr_104_hz, fs, lsm6dso::odr_104_hz, lsm6dso::gyro_250_dps, samples, sizeof(samples) / sizeof(samples[0]));
    vTaskDelay(pdMS_TO_TICKS(10 * 1000));
    assert(peripherals::screwdriver.arm() == true);
    ESP_LOGI(TAG, "Screwdriver on");
    assert(peripherals::screwdriver.dump() == true);
    ESP_LOGI(TAG, "Screwdriver off");
}
