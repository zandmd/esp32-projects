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
    static int radio_counter = 0;
    static TaskHandle_t radio_task = nullptr;
    peripherals::buttons.buttonchange = [](bool state, int buttonnum) {
        ESP_LOGI(TAG, "Button Number: %d Button State %d", buttonnum, state);
        if (state == true) {
            peripherals::leds.change_leds(buttonnum,leds::booting);
            if (buttonnum == 5 && radio_task == nullptr) {
                assert(xTaskCreate([](void *) {
                    ESP_LOGI(TAG, "Init RX task");
                    while (true) {
                        int counter = -1;
                        peripherals::lora >> counter;
                        ESP_LOGI(TAG, "RX packet: %d", counter);
                    }
                }, "radio task", 0x1000, nullptr, 2, &radio_task) == pdPASS);
            }
        } else {
            peripherals::leds.change_leds(buttonnum,leds::armed);
            if (buttonnum == 0 && radio_task == nullptr) {
                ESP_LOGI(TAG, "TX packet: %d", radio_counter);
                peripherals::lora << radio_counter++;
            }
        }
    };
    peripherals::lora.enable();
}
