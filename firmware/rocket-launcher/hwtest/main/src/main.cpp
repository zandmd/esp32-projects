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
    peripherals::buttons.buttonchange = [](bool state, int buttonnum){
        ESP_LOGI(TAG,"Button Number: %d Button State %d",buttonnum,state);
        if(state == true) {
            peripherals::leds.change_leds(buttonnum,leds::booting);
        }
        else {
            peripherals::leds.change_leds(buttonnum,leds::armed);
        }
        
    };
}
