#ifndef ZANDMD_BSP_ANALOG_HPP
#define ZANDMD_BSP_ANALOG_HPP

#include <zandmd/drivers/adc_service.hpp>

namespace zandmd {
    namespace bsp {
        class analog {
            public:
                enum battery_state {
                    critical,
                    low,
                    good,
                    charged
                };

                analog() noexcept;

                float battery_voltage() const noexcept;
                float usb_voltage() const noexcept;

                battery_state battery() const noexcept;
                bool usb() const noexcept;

            private:
                static constexpr float battery_res_high = 5.1;
                static constexpr float battery_res_low = 10;
                static constexpr float usb_res_high = 5.1;
                static constexpr float usb_res_low = 10;

                static constexpr float usb_threshold_volt = 3.3;
                static constexpr float battery_charged_volt = 4.15;
                static constexpr float battery_critical_volt = 2.43 + 1.0 + 0.1; // brownout + dropout + margin
                static constexpr float battery_low_volt = battery_critical_volt + 0.3;

                drivers::adc_service adc_5v;
                drivers::adc_service adc_vbat;
        };
    }
}

#endif
