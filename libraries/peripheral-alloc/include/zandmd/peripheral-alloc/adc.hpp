#ifndef ZANDMD_PERIPHERAL_ALLOC_ADC_HPP
#define ZANDMD_PERIPHERAL_ALLOC_ADC_HPP

#include <utility>
#include <hal/adc_types.h>
#include <soc/gpio_num.h>
#include <zandmd/peripheral-alloc/detail/this_should_only_be_referenced_internally.hpp>
#include <zandmd/peripheral-alloc/generic_adc.hpp>

namespace zandmd {
    namespace peripheral_alloc {
        template <gpio_num_t Num>
        class adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&) {
                }
        };

        template <>
        class adc<GPIO_NUM_36> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_1, ADC_CHANNEL_0, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_37> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_1, ADC_CHANNEL_1, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_38> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_1, ADC_CHANNEL_2, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_39> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_1, ADC_CHANNEL_3, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_32> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_1, ADC_CHANNEL_4, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_33> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_1, ADC_CHANNEL_5, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_34> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_1, ADC_CHANNEL_6, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_35> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_1, ADC_CHANNEL_7, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_4> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_2, ADC_CHANNEL_0, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_0> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_2, ADC_CHANNEL_1, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_2> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_2, ADC_CHANNEL_2, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_15> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_2, ADC_CHANNEL_3, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_13> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_2, ADC_CHANNEL_4, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_12> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_2, ADC_CHANNEL_5, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_14> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_2, ADC_CHANNEL_6, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_27> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_2, ADC_CHANNEL_7, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_25> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_2, ADC_CHANNEL_8, std::move(tag)) {
                }
        };

        template <>
        class adc<GPIO_NUM_26> : public generic_adc {
            protected:
                constexpr adc(detail::this_should_only_be_referenced_internally &&tag) : generic_adc(ADC_UNIT_2, ADC_CHANNEL_9, std::move(tag)) {
                }
        };
    }
}

#endif
