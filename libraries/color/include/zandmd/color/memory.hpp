#ifndef ZANDMD_COLOR_MEMORY_HPP
#define ZANDMD_COLOR_MEMORY_HPP

#include <array>
#include <initializer_list>
#include <stddef.h>
#include <type_traits>

namespace zandmd {
    namespace color {
        template <typename type, size_t count, typename enable = void>
        class memory {
        };

        template <typename arithmetic_type, size_t count>
        class memory<arithmetic_type, count, typename std::enable_if<std::is_arithmetic<arithmetic_type>::value>::type> {
            public:
                template <typename... Values>
                constexpr memory(Values... values) noexcept : data({values...}) {
                }

                template <size_t n>
                constexpr arithmetic_type &get() noexcept {
                    return data[n];
                }

                template <size_t n>
                constexpr arithmetic_type get() const noexcept {
                    return data[n];
                }

            private:
                std::array<arithmetic_type, count> data;
        };
    }
}

#endif
