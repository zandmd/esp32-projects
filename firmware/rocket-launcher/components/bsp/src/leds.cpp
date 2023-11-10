#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>
#include <string.h>

#include <zandmd/bsp/leds.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/bsp/tasks.hpp>
#include <zandmd/color/color_cast.hpp>
#include <zandmd/color/color.hpp>
#include <zandmd/color/hsv.hpp>
#include <zandmd/drivers/ws2811.hpp>

using namespace zandmd::bsp;
using namespace zandmd::color;
using namespace zandmd::drivers;

leds::leds () noexcept {
    xTaskCreate(&leds::led_task, "leds", 0x1000, this, tasks::ledtask, NULL);
}

void leds::change_leds( int num, ledstate state) noexcept {
    ledstates[num] = state;
}

void leds::change_all(ledstate state) noexcept {
    for (int i = 0; i < 4; ++i) {
        change_leds(i, state);
    }
}

void leds::led_task (void * context) noexcept {

    leds* led = (leds*)context;
    ws2811::color_rgb ledcolor[11];

    while (1) {
        for (int i =0; i < 11; i++) {
            ledcolor[i] = ws2811::color_rgb(0,0,0);
        }

        TickType_t tick = xTaskGetTickCount();

        for (int i = 0; i < 4; i++) {
            if (led->ledstates[i] == booting) {
                ledcolor[i] = ws2811::color_rgb(0,0,0);
            }

            if(led->ledstates[i] == commsfail) {
                if((tick/pdMS_TO_TICKS(1000))%2 == 0) {
                    ledcolor[i] = ws2811::color_rgb(255,0,0);
                }
                else {
                    ledcolor[i] = ws2811::color_rgb(0,0,0);
                }
            }

            if(led->ledstates[i] == lowbat) {
                ledcolor[i] = ws2811::color_rgb(255-5*abs((int)(tick%100-50)),0,0);
            }

            if(led->ledstates[i] == batterywrong) {
                if((tick/pdMS_TO_TICKS(100))%2 == 0) {
                    ledcolor[i] = ws2811::color_rgb(255,0,0);
                }
                else {
                    ledcolor[i] = ws2811::color_rgb(0,0,0);
                }
            }

            if(led->ledstates[i] == armed) {
                ledcolor[i] = color_cast<ws2811::color_rgb::format,ws2811::color_rgb::rep>(color::color<hsv,uint8_t>(tick,255,255));
            }

            if(led->ledstates[i] == open) {
                ledcolor[i] = ws2811::color_rgb(255,100,0);
            }

            if(led->ledstates[i] == closed) {
                ledcolor[i] = ws2811::color_rgb(0,255,0);
            }

        }

        if (peripherals::buttons.get_button_state(5) == 0) {
            ledcolor[8] = ledcolor[2];
            ledcolor[2] = ledcolor[1];
            ledcolor[10] = ledcolor[3];
            ledcolor[1] = ws2811::color_rgb(0,0,0);
            ledcolor[3] = ws2811::color_rgb(0,0,0);
        }
        peripherals::led.start(ledcolor, 11);
        peripherals::led.wait();
        xTaskDelayUntil(&tick,1);
    }
}
