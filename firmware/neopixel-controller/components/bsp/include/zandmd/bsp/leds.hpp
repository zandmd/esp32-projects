#ifndef ZANDMD_BSP_LEDS_HPP
#define ZANDMD_BSP_LEDS_HPP

#include <array>
#include <stddef.h>
#include <zandmd/drivers/ws2811.hpp>

namespace zandmd {
    namespace bsp {
        class leds {
            public:
                using color = drivers::ws2811::color_rgb;

                static constexpr size_t NUM_STRANDS = 8;
                static constexpr size_t LEDS_PER_STRAND = 250;

                leds() noexcept;

                void update() noexcept;

                void off() noexcept;
                bool color_wipe(const color &target, size_t counter) noexcept;

                std::array<std::array<color, LEDS_PER_STRAND>, NUM_STRANDS> data;

            private:
                static constexpr size_t CONCURRENT_STRANDS = 2;

                drivers::ws2811 led0;
                drivers::ws2811 led1;
                drivers::ws2811 led2;
                drivers::ws2811 led3;
                drivers::ws2811 led4;
                drivers::ws2811 led5;
                drivers::ws2811 led6;
                drivers::ws2811 led7;
        };
    }
}

#endif
