#ifndef ZANDMD_PERIPHERAL_ALLOC_GPIO_HPP
#define ZANDMD_PERIPHERAL_ALLOC_GPIO_HPP

#include <utility>
#include <soc/gpio_num.h>
#include <zandmd/peripheral-alloc/detail/this_should_only_be_referenced_internally.hpp>
#include <zandmd/peripheral-alloc/adc.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

namespace zandmd {
    namespace peripheral_alloc {
        template <gpio_num_t Num>
        class gpio : public generic_gpio, public adc<Num> {
            public:
                explicit constexpr gpio(detail::this_should_only_be_referenced_internally &&tag) noexcept : generic_gpio(Num, std::move(tag)), adc<Num>(std::move(tag)) {
                }
        };
    }
}


#endif
