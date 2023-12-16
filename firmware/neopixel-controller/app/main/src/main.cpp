#include <stddef.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/rtos_init.hpp>
#include <zandmd/graphics/multi_span.hpp>

using namespace zandmd::bsp;
using namespace zandmd::drivers;
using namespace zandmd::graphics;

extern "C" void app_main() {
    if (xTaskGetTickCount() > pdMS_TO_TICKS(60 * 1000)) { // Weird linker issue
        rtos_init::post_init();
    }
    peripherals::lights.all.stripe({
        multi_span::value_type(127, 0, 255),
        multi_span::value_type(0, 0, 255),
        multi_span::value_type(255, 127, 0),
        multi_span::value_type(255, 0, 0),
        multi_span::value_type(0, 255, 0),
        multi_span::value_type(191, 255, 63)
    }, 0);
    peripherals::lights.update();
}
