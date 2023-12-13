#ifndef ZANDM_GRAPHICS_BUFFER_HPP
#define ZANDM_GRAPHICS_BUFFER_HPP

#include <stddef.h>
#include <zandmd/graphics/null_span.hpp>
#include <zandmd/graphics/span.hpp>

namespace zandmd {
    namespace graphics {
        template <size_t count>
        class buffer : public span {
            public:
                constexpr buffer() : span(array, count) {
                }

            private:
                value_type array[count];
        };

        template <>
        class buffer<0> : public null_span {
        };
    }
}

#endif
