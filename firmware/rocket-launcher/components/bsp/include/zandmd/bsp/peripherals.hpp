#ifndef ZANDMD_BSP_PERIPHERALS_HPP
#define ZANDMD_BSP_PERIPHERALS_HPP

#include <zandmd/bsp/button.hpp>
#include <zandmd/drivers/lora.hpp>
#include <zandmd/drivers/ws2811.hpp>
#include <zandmd/bsp/leds.hpp>

namespace zandmd {
    namespace bsp {
        class peripherals {
            public:
                static drivers::ws2811 led;
                static button buttons;
                static bsp::leds leds;
                static drivers::lora lora;

            private:
                peripherals();
        };
    }
}

#endif
