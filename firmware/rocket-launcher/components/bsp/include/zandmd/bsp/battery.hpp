#ifndef ZANDMD_BSP_BATTERY_HPP
#define ZANDMD_BSP_BATTERY_HPP

#include <zandmd/drivers/adc_service.hpp>
#include <zandmd/peripheral-alloc/generic_adc.hpp>

namespace zandmd {
    namespace bsp {
        class battery {
            public:
                enum state {
                    backwards,
                    low,
                    good
                };

                explicit battery(peripheral_alloc::generic_adc adc) noexcept;

                int poll_raw() const noexcept;
                state poll() const noexcept;

            private:
                drivers::adc_service adc;
        };
    }
}

#endif
