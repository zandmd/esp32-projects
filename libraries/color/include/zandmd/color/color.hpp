#ifndef ZANDMD_COLOR_COLOR_HPP
#define ZANDMD_COLOR_COLOR_HPP

#include <stddef.h>
#include <tuple>

namespace zandmd {
    namespace color {
        template <typename format, typename rep>
        class color {
            private:
                color() noexcept;
        };
    }
}

namespace std {
    template <size_t n, typename format, typename rep>
    constexpr typename tuple_element<n, zandmd::color::color<format, rep>>::type &get(zandmd::color::color<format, rep> &c) noexcept {
        return tuple_element<n, zandmd::color::color<format, rep>>::get(c);
    }

    template <size_t n, typename format, typename rep>
    constexpr typename tuple_element<n, zandmd::color::color<format, rep>>::type get(const zandmd::color::color<format, rep> &c) noexcept {
        return tuple_element<n, zandmd::color::color<format, rep>>::get(c);
    }
}

#endif
