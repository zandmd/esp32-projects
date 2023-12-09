#ifndef ZANDMD_BSP_PERIPHERALS_HPP
#define ZANDMD_BSP_PERIPHERALS_HPP

#include <zandmd/bsp/leds.hpp>

namespace zandmd {
    namespace bsp {
        class peripherals {
            public:
                static leds lights;

            private:
                peripherals() noexcept;
        };
    }
}

#endif
