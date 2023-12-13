#ifndef ZANDMD_BSP_LEDS_HPP
#define ZANDMD_BSP_LEDS_HPP

#include <stddef.h>
#include <zandmd/drivers/ws2811.hpp>
#include <zandmd/graphics/array_span.hpp>
#include <zandmd/graphics/buffer.hpp>
#include <zandmd/graphics/span.hpp>

namespace zandmd {
    namespace bsp {
        class leds {
            public:
                static constexpr size_t NUM_STRANDS = 8;

                leds() noexcept;

                void update() noexcept;

            private:
                graphics::buffer<0> buffer0;
                graphics::buffer<0> buffer1;
                graphics::buffer<0> buffer2;
                graphics::buffer<0> buffer3;
                graphics::buffer<0> buffer4;
                graphics::buffer<0> buffer5;
                graphics::buffer<69> buffer6;
                graphics::buffer<212> buffer7;

            public:
                graphics::span garage_gutter_l2r;
                graphics::span porch_gutter_l2r;
                graphics::span sidewalk_b2f;

                graphics::array_span<3> all;

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
