#ifndef ZANDMD_BSP_PERIPHERALS_HPP
#define ZANDMD_BSP_PERIPHERALS_HPP

#include <zandmd/drivers/ws2811.hpp>

namespace zandmd {
    namespace bsp {
        class peripherals {
            public:
                static drivers::ws2811 led;

            private:
                peripherals();
        };
    }
}

#endif
