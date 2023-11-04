#ifndef ZANDMD_BSP_PERIPHERALS_HPP
#define ZANDMD_BSP_PERIPHERALS_HPP

#include <zandmd/bsp/button.hpp>
#include <zandmd/drivers/ws2811.hpp>

namespace zandmd {
    namespace bsp {
        class peripherals {
            public:
                static drivers::ws2811 led;
                static button buttons;

            private:
                peripherals();
        };
    }
}

#endif
