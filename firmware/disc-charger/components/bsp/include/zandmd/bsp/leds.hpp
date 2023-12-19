#ifndef ZANDMD_BSP_LEDS_HPP
#define ZANDMD_BSP_LEDS_HPP

#include <stdint.h>

namespace zandmd {
    namespace bsp {
        class leds {
            public:
                leds() noexcept;

                void off() noexcept;
                void on(float duty_cycle) noexcept;

            private:
                static constexpr uint32_t frequency = 25000;
        };
    }
}

#endif
