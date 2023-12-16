#include <stddef.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/rtos_init.hpp>
#include <zandmd/graphics/multi_span.hpp>

using namespace zandmd::bsp;
using namespace zandmd::drivers;
using namespace zandmd::graphics;

#define SEQUENCE { \
    multi_span::value_type(0, 0, 0), \
    multi_span::value_type(127, 0, 0), \
    multi_span::value_type(255, 0, 0), \
    multi_span::value_type(255, 0, 0), \
    multi_span::value_type(255, 0, 0), \
    multi_span::value_type(127, 0, 0), \
    multi_span::value_type(0, 0, 0), \
    multi_span::value_type(0, 127, 0), \
    multi_span::value_type(0, 255, 0), \
    multi_span::value_type(0, 255, 0), \
    multi_span::value_type(0, 255, 0), \
    multi_span::value_type(0, 127, 0), \
    multi_span::value_type(0, 0, 0), \
    multi_span::value_type(95, 127, 31), \
    multi_span::value_type(191, 255, 63), \
    multi_span::value_type(191, 255, 63), \
    multi_span::value_type(191, 255, 63), \
    multi_span::value_type(95, 127, 31) \
}

extern "C" void app_main() {
    if (xTaskGetTickCount() > pdMS_TO_TICKS(60 * 1000)) { // Weird linker issue
        rtos_init::post_init();
    }
    while (true) {
        size_t i;
        for (i = 0; i < 2 * 60 * 10; ++i) {
            peripherals::lights.all.stripe(SEQUENCE, i);
            peripherals::lights.update();
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        for (; i > 0; --i) {
            peripherals::lights.all.stripe(SEQUENCE, i);
            peripherals::lights.update();
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}
