#include <algorithm>
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

using namespace std;
using namespace zandmd::bsp;
using namespace zandmd::color;
using namespace zandmd::drivers;

enum class states {
    idle,
    arming,
    armed,
    fired
};

extern "C" void app_main() {
    StaticSemaphore_t semaphore_mem;
    SemaphoreHandle_t semaphore = xSemaphoreCreateBinaryStatic(&semaphore_mem);
    states state = states::idle;
    peripherals::button.enable();
    peripherals::button.callback = [&state, semaphore](button::event ev, TickType_t time) {
        switch (ev) {
            case button::press:
                if (state != states::fired) {
                    ESP_LOGI(TAG, "idle: button pressed");
                    state = states::idle;
                    peripherals::screwdriver.disarm();
                    xSemaphoreGive(semaphore);
                }
                break;
            case button::triple_click:
                if (state == states::idle) {
                    ESP_LOGI(TAG, "arming: button pressed");
                    state = states::arming;
                    xSemaphoreGive(semaphore);
                }
                break;
            default:
                break;
        }
    };
    lsm6dso::accel_fs fs = lsm6dso::accel_4_g;
    unsigned counter = 0;
    peripherals::imu.callback = [&state, fs, &counter, semaphore](const lsm6dso::sample *samples, size_t count) {
        // Sitting normally: <0, 0, 1>
        // Sitting on motor: <0, -1, 0>
        float sum[3] = {0, 0, 0};
        size_t accels = 0;
        for (size_t i = 0; i < count; ++i) {
            if (samples[i].tag.sensor() == lsm6dso::sample::tag_type::accel_nc) {
                sum[0] += samples[i].x.accel(fs);
                sum[1] += samples[i].y.accel(fs);
                sum[2] += samples[i].z.accel(fs);
                ++accels;
            }
        }
        float avg[3] = {
            sum[0] / static_cast<float>(accels),
            sum[1] / static_cast<float>(accels),
            sum[2] / static_cast<float>(accels)
        };
        if (++counter % 5 == 0) {
            ESP_LOGI(TAG, "imu: <%.3f, %.3f, %.3f>", avg[0], avg[1], avg[2]);
        }
        bool consistent = true;
        int trigger_count = 0;
        bool triggered = false;
        for (size_t i = 0; i < count; ++i) {
            if (samples[i].tag.sensor() == lsm6dso::sample::tag_type::accel_nc) {
                float x = samples[i].x.accel(fs);
                float y = samples[i].y.accel(fs);
                float z = samples[i].z.accel(fs);
                float dx = avg[0] - x;
                float dy = avg[1] - y;
                float dz = avg[2] - z;
                if (dx * dx + dy * dy + dz * dz > 0.3f * 0.3f) {
                    consistent = false;
                    if (triggered) {
                        break;
                    }
                }
                if (abs(x) < 0.1f && abs(y) < 0.1f && abs(z) > 0.4f) {
                    if (++trigger_count > 5) {
                        triggered = true;
                        if (!consistent) {
                            break;
                        }
                    }
                }
            }
        }
        switch (state) {
            case states::arming:
                if (consistent && !triggered && abs(avg[0]) < 0.1f && avg[1] < -0.3f && avg[2] > 0.4f) {
                    ESP_LOGI(TAG, "armed: imu <%.3f, %.3f, %.3f> (%d, %d)", avg[0], avg[1], avg[2], consistent, triggered);
                    state = states::armed;
                    peripherals::screwdriver.arm();
                } else {
                    ESP_LOGI(TAG, "idle: imu <%.3f, %.3f, %.3f> (%d, %d)", avg[0], avg[1], avg[2], consistent, triggered);
                    state = states::idle;
                }
                break;
            case states::armed:
                if (triggered) {
                    ESP_LOGI(TAG, "fired: imu <%.3f, %.3f, %.3f> (%d, %d)", avg[0], avg[1], avg[2], consistent, triggered);
                    state = states::fired;
                    xSemaphoreGive(semaphore);
                } else if (abs(avg[0]) > 0.1f || avg[1] > 0.1f || avg[2] < 0.4f) {
                    ESP_LOGI(TAG, "idle: imu <%.3f, %.3f, %.3f> (%d, %d)", avg[0], avg[1], avg[2], consistent, triggered);
                    state = states::idle;
                    peripherals::screwdriver.disarm();
                    xSemaphoreGive(semaphore);
                }
                break;
            default:
                break;
        }
    };
    static lsm6dso::sample samples[150];
    peripherals::imu.enable(4, lsm6dso::odr_208_hz, fs, lsm6dso::odr_208_hz, lsm6dso::gyro_250_dps, samples, sizeof(samples) / sizeof(samples[0]));
    ws2811::color_grb off = color_cast<ws2811::color_grb::format, ws2811::color_grb::rep>(color<rgb, uint8_t>(0x00, 0x00, 0x00));
    ws2811::color_grb red = color_cast<ws2811::color_grb::format, ws2811::color_grb::rep>(color<rgb, uint8_t>(0xFF, 0x00, 0x00));
    ws2811::color_grb green = color_cast<ws2811::color_grb::format, ws2811::color_grb::rep>(color<rgb, uint8_t>(0x00, 0xFF, 0x00));
    ws2811::color_grb blue = color_cast<ws2811::color_grb::format, ws2811::color_grb::rep>(color<rgb, uint8_t>(0x00, 0x00, 0xFF));
    while (true) {
        switch (state) {
            case states::idle:
                peripherals::led.start(&red, 1);
                peripherals::led.wait();
                xSemaphoreTake(semaphore, portMAX_DELAY);
                break;
            case states::arming:
            case states::armed:
                peripherals::led.start(&green, 1);
                vTaskDelay(pdMS_TO_TICKS(50));
                peripherals::led.start(&off, 1);
                vTaskDelay(pdMS_TO_TICKS(50));
                peripherals::led.start(&green, 1);
                vTaskDelay(pdMS_TO_TICKS(50));
                peripherals::led.start(&off, 1);
                vTaskDelay(pdMS_TO_TICKS(50));
                peripherals::led.start(&green, 1);
                vTaskDelay(pdMS_TO_TICKS(50));
                peripherals::led.start(&off, 1);
                vTaskDelay(pdMS_TO_TICKS(50));
                xSemaphoreTake(semaphore, pdMS_TO_TICKS(1000 - 50 * 6));
                break;
            case states::fired:
                peripherals::led.start(&blue, 1);
                peripherals::led.wait();
                peripherals::screwdriver.dump();
                vTaskDelay(portMAX_DELAY);
                break;
            default:
                ESP_LOGE(TAG, "Invalid state: %d", static_cast<int>(state));
                assert(false);
                break;
        }
    }
}
