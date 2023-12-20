#ifndef ZANDMD_MATH_RANDOM_HPP
#define ZANDMD_MATH_RANDOM_HPP

#include <stdint.h>

namespace zandmd {
    namespace math {
        class random {
            public:
                explicit random(uint32_t state = 0x6034740D) noexcept;

                operator uint32_t() noexcept;

            private:
                uint32_t state;
        };
    }
}

#endif
