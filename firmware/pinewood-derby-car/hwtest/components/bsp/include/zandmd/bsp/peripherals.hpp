#ifndef ZANDMD_BSP_PERIPHERALS_HPP
#define ZANDMD_BSP_PERIPHERALS_HPP

#include <zandmd/drivers/button.hpp>
#include <zandmd/drivers/lsm6dso.hpp>
#include <zandmd/drivers/power_dump.hpp>
#include <zandmd/drivers/ws2811.hpp>

namespace zandmd {
    namespace bsp {
        class peripherals {
            public:
                static drivers::button button;
                static drivers::ws2811 led;
                static drivers::power_dump screwdriver;
                static drivers::lsm6dso imu;

            private:
                peripherals();
        };
    }
}

#endif
