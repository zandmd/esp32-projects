#include <array>
#include <stddef.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/rtos_init.hpp>
#include <zandmd/graphics/multi_span.hpp>
#include <zandmd/math/random.hpp>

using namespace std;
using namespace zandmd;
using namespace zandmd::bsp;
using namespace zandmd::drivers;
using namespace zandmd::graphics;

extern "C" void app_main() {
    if (xTaskGetTickCount() > pdMS_TO_TICKS(60 * 1000)) { // Weird linker issue
        rtos_init::post_init();
    }
    math::random rand;
    while (true) {
        while (peripherals::lights.all.random(multi_span::value_type(255, 0, 0), rand)) {
            peripherals::lights.update();
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        while (peripherals::lights.all.random(multi_span::value_type(0, 255, 0), rand)) {
            peripherals::lights.update();
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}
