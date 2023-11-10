#ifndef ZANDMD_DRIVERS_ADC_SERVICE_HPP
#define ZANDMD_DRIVERS_ADC_SERVICE_HPP

#include <esp_adc/adc_oneshot.h>
#include <hal/adc_types.h>
#include <soc/soc_caps.h>
#include <zandmd/peripheral-alloc/generic_adc.hpp>

namespace zandmd {
    namespace drivers {
        class adc_service {
            public:
                explicit adc_service(peripheral_alloc::generic_adc pin, adc_bitwidth_t width = ADC_BITWIDTH_12, adc_atten_t atten = ADC_ATTEN_DB_11) noexcept;
                ~adc_service() noexcept;

                int poll() const noexcept;

            private:
                struct unit {
                    unit() noexcept;

                    adc_oneshot_unit_handle_t handle;
                    unsigned ref_count;
                };

                static unit units[SOC_ADC_DIGI_CONTROLLER_NUM];
                peripheral_alloc::generic_adc pin;
        };
    }
}

#endif
