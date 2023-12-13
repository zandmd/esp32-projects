#ifndef ZANDMD_GRAPHICS_ARRAY_SPAN_HPP
#define ZANDMD_GRAPHICS_ARRAY_SPAN_HPP

#include <initializer_list>
#include <stddef.h>
#include <tuple>
#include <zandmd/graphics/span.hpp>
#include <zandmd/graphics/multi_span.hpp>

namespace zandmd {
    namespace graphics {
        template <size_t count>
        class array_span : public multi_span {
            public:
                constexpr array_span(const std::initializer_list<span *> &spans) : multi_span(data, count) {
                    auto it = data;
                    for (auto &span : spans) {
                        *it++ = std::tuple<pointer, pointer>(span->begin(), span->end());
                    }
                    *it = std::tuple<pointer, pointer>(nullptr, nullptr);
                }

            private:
                std::tuple<pointer, pointer> data[count + 1];
        };
    }
}

#endif
