#ifndef ZANDMD_BSP_LEDS_HPP
#define ZANDMD_BSP_LEDS_HPP

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
                    open, // orange
                    closed //green
                };

                leds() noexcept;
                void change_leds(int num, ledstate state) noexcept;
                void change_all(ledstate state) noexcept;
            private:
                static void led_task(void * context) noexcept;
                ledstate ledstates[4];
        };
    }
}

#endif
