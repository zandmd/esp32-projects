#include <assert.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
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

leds::leds () noexcept : watchdog(false) {
    sem = xSemaphoreCreateRecursiveMutexStatic(&sem_mem);
    assert(sem != nullptr);
    task = xTaskCreateStatic(&leds::led_task, "leds", sizeof(stack), this, tasks::ledtask, stack, &task_mem);
    assert(task != nullptr);
}

leds::~leds() noexcept {
    if (watchdog) {
        ESP_ERROR_CHECK(esp_task_wdt_delete(task));
    }
    vTaskDelete(task);
    vSemaphoreDelete(sem);
}

void leds::enable_watchdog() noexcept {
    ESP_ERROR_CHECK(esp_task_wdt_add(task));
    watchdog = true;
}

void leds::change_leds( int num, ledstate state) noexcept {
    assert(xSemaphoreTakeRecursive(sem, portMAX_DELAY) == pdTRUE);
    ledstates[num] = state;
    assert(xSemaphoreGiveRecursive(sem) == pdTRUE);
}

void leds::change_all(ledstate state) noexcept {
    assert(xSemaphoreTakeRecursive(sem, portMAX_DELAY) == pdTRUE);
    for (int i = 0; i < 4; ++i) {
        change_leds(i, state);
    }
    assert(xSemaphoreGiveRecursive(sem) == pdTRUE);
}

void leds::led_task (void * context) noexcept {

    leds* led = (leds*)context;
    ws2811::color_rgb ledcolor[11];

    while (1) {
        if (led->watchdog) {
            ESP_ERROR_CHECK(esp_task_wdt_reset());
        }

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

            if (led->ledstates[i] == latched) {
                ledcolor[i] = ws2811::color_rgb(255, 31, 0);
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
