#include <array>
#include <stddef.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/leds.hpp>
#include <zandmd/bsp/peripherals.hpp>

#define TAG "main"

using namespace std;
using namespace zandmd::bsp;

extern "C" void app_main() {
    constexpr array<leds::color, 3> colors = {
        leds::color(63, 0, 0),
        leds::color(0, 63, 0),
        leds::color(0, 0, 63)
    };
    TickType_t next_update = xTaskGetTickCount() + pdMS_TO_TICKS(1000);
    size_t frames = 0;
    while (true) {
        for (const auto &color : colors) {
            for (size_t i = 0; peripherals::lights.color_wipe(color, i); ++i) {
                peripherals::lights.update();
                ++frames;
                if (xTaskGetTickCount() >= next_update) {
                    next_update += pdMS_TO_TICKS(1000);
                    ESP_LOGI(TAG, "Frame rate: %d FPS", frames);
                    frames = 0;
                }
            }
        }
    }
}
