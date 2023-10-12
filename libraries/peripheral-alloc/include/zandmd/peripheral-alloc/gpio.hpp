#ifndef ZANDMD_PERIPHERAL_ALLOC_GPIO_HPP
#define ZANDMD_PERIPHERAL_ALLOC_GPIO_HPP

#include <utility>
#include <soc/gpio_num.h>
#include <zandmd/peripheral-alloc/detail/this_should_only_be_referenced_internally.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

#ifndef ZANDMD_BSP_GPIO_HPP

namespace zandmd {
    namespace peripheral_alloc {
        template <gpio_num_t Num>
        class gpio : public generic_gpio {
            public:
                explicit constexpr gpio(detail::this_should_only_be_referenced_internally &&tag) noexcept : generic_gpio(Num, std::move(tag)) {
                }
        };
    }
}

#else

#ifndef ZANDMD_BSP_GPIO_HEADER
#define ZANDMD_BSP_GPIO_HEADER

#undef ZANDMD_PERIPHERAL_ALLOC_GPIO_HPP
#undef ZANDMD_BSP_GPIO_HPP
#include <zandmd/peripheral-alloc/gpio.hpp>
#define ZANDMD_BSP_GPIO_HPP

#ifndef ZANDMD_PERIPHERAL_ALLOC_SRC_GPIO_CPP
namespace zandmd {
    namespace bsp {
        class gpio {
            public:

#define ZANDMD_GPIO(name, pin) \
                static constexpr zandmd::peripheral_alloc::gpio<GPIO_NUM_ ## pin> name = zandmd::peripheral_alloc::gpio<GPIO_NUM_ ## pin>(zandmd::peripheral_alloc::detail::this_should_only_be_referenced_internally())
#endif
#undef ZANDMD_PERIPHERAL_ALLOC_GPIO_HPP

#else

#ifndef ZANDMD_PERIPHERAL_ALLOC_SRC_GPIO_CPP
        };
    }
}
#endif

#undef ZANDMD_GPIO

#endif
#endif

#endif
