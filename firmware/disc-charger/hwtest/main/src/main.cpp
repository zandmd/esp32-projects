#include <assert.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <zandmd/bsp/analog.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/color/color.hpp>
#include <zandmd/color/color_cast.hpp>
#include <zandmd/drivers/button.hpp>
#include <zandmd/drivers/lsm6dso.hpp>
#include <zandmd/drivers/ws2811.hpp>

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
    peripherals::imu.enable(3, lsm6dso::odr_104_hz, fs, lsm6dso::odr_104_hz, lsm6dso::gyro_250_dps, samples, sizeof(samples) / sizeof(samples[0]));
    assert(xTaskCreate([](void *) {
        while (true) {
            static const char *battery_states[] = {
                "critical",
                "low",
                "good",
                "charged"
            };
            analog::battery_state batt = peripherals::adc.battery();
            if (batt < sizeof(battery_states) / sizeof(battery_states[0])) {
                ESP_LOGI(TAG, "ADC: battery: %.3f V (%s)", peripherals::adc.battery_voltage(), battery_states[batt]);
            } else {
                ESP_LOGI(TAG, "ADC: battery: %.3f V (%d)", peripherals::adc.battery_voltage(), batt);
            }
            ESP_LOGI(TAG, "ADC: USB: %.3f V (%s)", peripherals::adc.usb_voltage(), peripherals::adc.usb() ? "connected" : "disconnected");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }, "adc task", 0x1000, nullptr, 4, nullptr) == pdPASS);
    float led_duty = 0;
    while (true) {
        int ch = fgetc(stdin);
        if (ch >= '0' && ch <= '9') {
            float target_duty = static_cast<float>(ch - '0') / 9.f;
            if (target_duty != led_duty) {
                for (float step = 0; step < 1; step += 0.01f) {
                    peripherals::uv.on(led_duty + (target_duty - led_duty) * step);
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
                if (target_duty == 0) {
                    peripherals::uv.off();
                } else {
                    peripherals::uv.on(target_duty);
                }
                led_duty = target_duty;
            }
        }
        vTaskDelay(1);
    }
}
