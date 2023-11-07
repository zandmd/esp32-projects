#include <assert.h>
#include <stdint.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/button.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/bsp/tasks.hpp>
#include <zandmd/color/color_cast.hpp>
#include <zandmd/color/color.hpp>
#include <zandmd/color/hsv.hpp>
#include <zandmd/drivers/ws2811.hpp>

#define TAG "main"
#define NUMLEDS 11

using namespace zandmd::bsp;
using namespace zandmd::color;
using namespace zandmd::drivers;

extern "C" void app_main() {
    assert(xTaskCreate([](void *) {
        color<hsv, uint8_t> color(0, 255, 15);
        while (true) {
            ++color.hue();
            ws2811::color_rgb converted[NUMLEDS];
            
            for (int i = 0; i < NUMLEDS; i++) {
                converted[i] = color_cast<ws2811::color_rgb::format, ws2811::color_rgb::rep>(color);
            } 

            peripherals::led.start(converted, 11);
            peripherals::led.wait();
            vTaskDelay(pdMS_TO_TICKS(10));


        }
    }, "led task", 0x1000, nullptr, tasks::ledtask, nullptr) == pdPASS);

    peripherals::buttons.buttonchange = [](bool state, int buttonnum){
        ESP_LOGI(TAG,"Button Number: %d Button State %d",buttonnum,state);
    };
}
