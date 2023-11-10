#ifndef ZANDMD_PERIPHERAL_ALLOC_GENERIC_ADC_HPP
#define ZANDMD_PERIPHERAL_ALLOC_GENERIC_ADC_HPP

#include <hal/adc_types.h>
#include <zandmd/peripheral-alloc/detail/this_should_only_be_referenced_internally.hpp>

namespace zandmd {
    namespace peripheral_alloc {
        class generic_adc {
            public:
                constexpr operator adc_unit_t() const noexcept {
                    return unit;
                }

                constexpr operator adc_channel_t() const noexcept {
                    return channel;
                }

            protected:
                constexpr generic_adc(adc_unit_t unit, adc_channel_t channel, detail::this_should_only_be_referenced_internally &&) noexcept : unit(unit), channel(channel) {
                }

            private:
                adc_unit_t unit;
                adc_channel_t channel;
        };
    }
}

#endif
