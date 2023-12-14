#ifndef ZANDMD_BSP_PERIPHERALS_HPP
#define ZANDMD_BSP_PERIPHERALS_HPP

#include <zandmd/bsp/leds.hpp>
#include <zandmd/drivers/event_loop.hpp>
#include <zandmd/drivers/nonvol.hpp>
#include <zandmd/wifi/station.hpp>

namespace zandmd {
    namespace bsp {
        class peripherals {
            public:
                static drivers::event_loop event_loop;
                static drivers::nonvol nonvol;
                static wifi::station wifi;
                static leds lights;

            private:
                peripherals() noexcept;
        };
    }
}

#endif
