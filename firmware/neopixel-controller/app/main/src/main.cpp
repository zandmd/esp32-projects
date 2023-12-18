#include <array>
#include <stddef.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/rtos_init.hpp>
#include <zandmd/graphics/multi_span.hpp>

using namespace std;
using namespace zandmd::bsp;
using namespace zandmd::drivers;
using namespace zandmd::graphics;

extern "C" void app_main() {
    if (xTaskGetTickCount() > pdMS_TO_TICKS(60 * 1000)) { // Weird linker issue
        rtos_init::post_init();
    }
    constexpr array<multi_span::value_type, 3> colors = {
        multi_span::value_type(255, 0, 0),
        multi_span::value_type(0, 255, 0),
        multi_span::value_type(191, 255, 63)
    };
    while (true) {
        for (const auto &color : colors) {
            for (size_t i = 0; peripherals::lights.garage_gutter_l2r.color_wipe(color, i, multi_span::center); ++i) {
                peripherals::lights.update();
                vTaskDelay(pdMS_TO_TICKS(10));
            }
            for (size_t i = 0; peripherals::lights.porch_gutter_l2r.color_wipe(color, i, multi_span::finish); ++i) {
                peripherals::lights.update();
                vTaskDelay(pdMS_TO_TICKS(10));
            }
            for (size_t i = 0; peripherals::lights.sidewalk_b2f.color_wipe(color, i, multi_span::start); ++i) {
                peripherals::lights.update();
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
    }
}
