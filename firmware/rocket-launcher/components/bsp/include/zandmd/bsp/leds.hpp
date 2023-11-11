#ifndef ZANDMD_BSP_LEDS_HPP
#define ZANDMD_BSP_LEDS_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

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
                void change_leds(int num, ledstate state) noexcept;
                void change_all(ledstate state) noexcept;
            private:
                static void led_task(void * context) noexcept;
                ledstate ledstates[4];
                StaticSemaphore_t sem_mem;
                SemaphoreHandle_t sem;
        };
    }
}

#endif
