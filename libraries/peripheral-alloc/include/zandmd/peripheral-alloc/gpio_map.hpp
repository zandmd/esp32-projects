#ifndef ZANDMD_BSP_GPIO_HPP
#error <zandmd/bsp/gpio_map.hpp> should only be included by <zandmd/bsp/gpio.hpp>
#endif

#ifndef ZANDMD_PERIPHERAL_ALLOC_GPIO_MAP_HPP
#define ZANDMD_PERIPHERAL_ALLOC_GPIO_MAP_HPP

#include <soc/gpio_num.h>
#include <zandmd/peripheral-alloc/gpio.hpp>
#include <zandmd/peripheral-alloc/detail/this_should_only_be_referenced_internally.hpp>

#ifndef ZANDMD_PERIPHERAL_ALLOC_SRC_GPIO_CPP
namespace zandmd {
    namespace bsp {
        class gpio {
            public:

#define ZANDMD_GPIO(name, pin) \
                static constexpr zandmd::peripheral_alloc::gpio<GPIO_NUM_ ## pin> name = zandmd::peripheral_alloc::gpio<GPIO_NUM_ ## pin>(zandmd::peripheral_alloc::detail::this_should_only_be_referenced_internally())
#endif

#else

#ifndef ZANDMD_PERIPHERAL_ALLOC_SRC_GPIO_CPP
        };
    }
}
#endif

#undef ZANDMD_GPIO

#endif
