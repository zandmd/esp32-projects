#ifndef ZANDMD_BSP_PERIPHERALS_HPP
#define ZANDMD_BSP_PERIPHERALS_HPP

#include <zandmd/drivers/button.hpp>

namespace zandmd {
    namespace bsp {
        class peripherals {
            public:
                static drivers::button btn;

            private:
                peripherals() noexcept;
        };
    }
}

#endif
