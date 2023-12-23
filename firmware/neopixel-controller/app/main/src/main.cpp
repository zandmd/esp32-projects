#include <array>
#include <stddef.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/color/color.hpp>
#include <zandmd/color/hsv.hpp>
#include <zandmd/drivers/rtos_init.hpp>
#include <zandmd/graphics/multi_span.hpp>
#include <zandmd/math/random.hpp>

using namespace std;
using namespace zandmd;
using namespace zandmd::bsp;
using namespace zandmd::color;
using namespace zandmd::drivers;
using namespace zandmd::graphics;

extern "C" void app_main() {
    if (xTaskGetTickCount() > pdMS_TO_TICKS(60 * 1000)) { // Weird linker issue
        rtos_init::post_init();
    }
    size_t i = 0;
    while (true) {
        peripherals::lights.all.rainbow_symmetric(zandmd::color::color<hsv, uint8_t>(4, 255, 255), 0, 85, 12, ++i);
        peripherals::lights.update();
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
