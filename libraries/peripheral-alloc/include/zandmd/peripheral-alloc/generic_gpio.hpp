#ifndef ZANDMD_PERIPHERAL_ALLOC_GENERIC_GPIO_HPP
#define ZANDMD_PERIPHERAL_ALLOC_GENERIC_GPIO_HPP

#include <soc/gpio_num.h>
#include <zandmd/peripheral-alloc/detail/this_should_only_be_referenced_internally.hpp>

namespace zandmd {
    namespace peripheral_alloc {
        class generic_gpio {
            public:
                constexpr operator gpio_num_t() const noexcept {
                    return num;
                }

                constexpr operator int() const noexcept {
                    return num;
                }

            protected:
                constexpr generic_gpio(gpio_num_t num, detail::this_should_only_be_referenced_internally &&) noexcept : num(num) {
                }

            private:
                gpio_num_t num;
        };
    }
}

#endif
