#include <assert.h>
#include <stdint.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/color/color.hpp>
#include <zandmd/color/color_cast.hpp>
#include <zandmd/color/hsv.hpp>
#include <zandmd/drivers/ws2811.hpp>

#define TAG "main"

using namespace zandmd::bsp;
using namespace zandmd::color;
using namespace zandmd::drivers;

extern "C" void app_main() {
    assert(xTaskCreate([](void *) {
        color<hsv, uint8_t> color(0, 255, 15);
        while (true) {
            ++color.hue();
            ws2811::color converted = color_cast<ws2811::color::format, ws2811::color::rep>(color);
            peripherals::led.start(&converted, 1);
            peripherals::led.wait();
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }, "led task", 0x1000, nullptr, 3, nullptr) == pdPASS);
}
