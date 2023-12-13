#include <stddef.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/graphics/multi_span.hpp>

using namespace zandmd::bsp;
using namespace zandmd::graphics;

extern "C" void app_main() {
    for (size_t i = 0; ; ++i) {
        peripherals::lights.all.stripe({
            multi_span::value_type(0, 0, 0),
            multi_span::value_type(127, 0, 0),
            multi_span::value_type(255, 0, 0),
            multi_span::value_type(255, 0, 0),
            multi_span::value_type(255, 0, 0),
            multi_span::value_type(127, 0, 0),
            multi_span::value_type(0, 0, 0),
            multi_span::value_type(0, 127, 0),
            multi_span::value_type(0, 255, 0),
            multi_span::value_type(0, 255, 0),
            multi_span::value_type(0, 255, 0),
            multi_span::value_type(0, 127, 0),
            multi_span::value_type(0, 0, 0),
            multi_span::value_type(95, 127, 31),
            multi_span::value_type(191, 255, 63),
            multi_span::value_type(191, 255, 63),
            multi_span::value_type(191, 255, 63),
            multi_span::value_type(95, 127, 31)
        }, i);
        peripherals::lights.update();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
