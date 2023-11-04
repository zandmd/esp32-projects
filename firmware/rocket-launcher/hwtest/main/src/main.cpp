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
        color<hsv, uint8_t> color(0, 255, 255);
        while (true) {
            ++color.hue();
            ws2811::color_rgb converted = color_cast<ws2811::color_rgb::format, ws2811::color_rgb::rep>(color);
            peripherals::led.start(&converted, 1);
            peripherals::led.wait();
            vTaskDelay(pdMS_TO_TICKS(1000));
            ESP_LOGI(TAG,"color R:%d G:%d B%d",converted.red(),converted.green(),converted.blue());
        }
    }, "led task", 0x1000, nullptr, 3, nullptr) == pdPASS);
}
