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
        leds::color(255, 0, 0),
        leds::color(0, 255, 0),
        leds::color(191, 255, 63)
    };
    int j = 0;
    while (true) {
        int i = j++;
        for (auto &pixel : peripherals::lights.data[7]) {
            pixel = colors[i++ % 3];
        }
        for (auto &pixel : peripherals::lights.data[6]) {
            pixel = colors[i++ % 3];
        }
        for (i = 59; i < 107; ++i) {
            peripherals::lights.data[7][i] = leds::color(0, 0, 0);
        }
        for (i = 212; i < leds::LEDS_PER_STRAND; ++i) {
            peripherals::lights.data[7][i] = leds::color(0, 0, 0);
        }
        for (i = 69; i < leds::LEDS_PER_STRAND; ++i) {
            peripherals::lights.data[6][i] = leds::color(0, 0, 0);
        }
        peripherals::lights.update();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
