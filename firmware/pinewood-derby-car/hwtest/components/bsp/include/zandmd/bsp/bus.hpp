#ifndef ZANDMD_BSP_BUS_HPP
#define ZANDMD_BSP_BUS_HPP

#include <zandmd/bus/spi.hpp>
#include <zandmd/bus/spidev.hpp>

namespace zandmd {
    namespace bsp {
        class bus {
            public:
                static zandmd::bus::spi hspi;
                static zandmd::bus::spidev imu;

            private:
                bus();
        };
    }
}

#endif
