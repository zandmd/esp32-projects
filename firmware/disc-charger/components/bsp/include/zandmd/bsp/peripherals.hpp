#ifndef ZANDMD_BSP_PERIPHERALS_HPP
#define ZANDMD_BSP_PERIPHERALS_HPP

#include <zandmd/drivers/button.hpp>
#include <zandmd/drivers/ws2811.hpp>

namespace zandmd {
    namespace bsp {
        class peripherals {
            public:
                static drivers::button btn;
                static drivers::ws2811 status_led;

            private:
                peripherals() noexcept;
        };
    }
}

#endif
