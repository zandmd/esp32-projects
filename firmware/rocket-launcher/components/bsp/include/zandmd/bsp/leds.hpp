#ifndef ZANDMD_BSP_LEDS_HPP
#define ZANDMD_BSP_LEDS_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

namespace zandmd {
    namespace bsp {
        class leds {
            public:
                enum ledstate {
                    booting, //off
                    commsfail, //slow red blink
                    lowbat, //red fading
                    batterywrong, //fast red blink
                    armed, //rainbow
                    open, // yellow
                    closed, //green
                    latched // orange
                };

                leds() noexcept;
                ~leds() noexcept;

                void enable_watchdog() noexcept;

                void change_leds(int num, ledstate state) noexcept;
                void change_all(ledstate state) noexcept;

            private:
                static void led_task(void * context) noexcept;
                ledstate ledstates[4];
                StaticSemaphore_t sem_mem;
                SemaphoreHandle_t sem;
                StackType_t stack[4096];
                StaticTask_t task_mem;
                TaskHandle_t task;
                bool watchdog;
        };
    }
}

#endif
