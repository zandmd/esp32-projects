#ifndef ZANDMD_GRAPHICS_NULL_SPAN_HPP
#define ZANDMD_GRAPHICS_NULL_SPAN_HPP

#include <zandmd/graphics/multi_span.hpp>

namespace zandmd {
    namespace graphics {
        class null_span : public multi_span {
            public:
                constexpr null_span() noexcept : multi_span(&terminator, &terminator), terminator(nullptr, nullptr) {
                }

            private:
                std::tuple<pointer, pointer> terminator;
        };
    }
}

#endif
