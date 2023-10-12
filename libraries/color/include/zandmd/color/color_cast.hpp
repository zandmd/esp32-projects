#ifndef ZANDMD_COLOR_COLOR_CAST_HPP
#define ZANDMD_COLOR_COLOR_CAST_HPP

#include <algorithm>
#include <limits>
#include <stddef.h>
#include <stdint.h>
#include <tuple>
#include <type_traits>
#include <utility>
#include <zandmd/color/color.hpp>
#include <zandmd/color/hsv.hpp>
#include <zandmd/color/rgb.hpp>

namespace zandmd {
    namespace color {
        namespace detail {
            namespace color_cast {
                template <typename to_rep, typename from_rep>
                constexpr to_rep cast_rep_single(from_rep r) noexcept {
                    if constexpr (std::is_same<to_rep, from_rep>::value) {
                        return r;
                    }
                    if constexpr (std::is_floating_point<to_rep>::value) {
                        if constexpr (std::is_floating_point<from_rep>::value) {
                            return static_cast<to_rep>(r);
                        } else {
                            return static_cast<to_rep>(r) / static_cast<to_rep>(std::numeric_limits<from_rep>::max());
                        }
                    }
                    if constexpr (std::is_integral<to_rep>::value) {
                        if constexpr (std::is_floating_point<from_rep>::value) {
                            return static_cast<to_rep>(r * static_cast<from_rep>(std::numeric_limits<to_rep>::max()));
                        } else if constexpr (sizeof(to_rep) < sizeof(from_rep)) {
                            return static_cast<to_rep>(r >> ((sizeof(from_rep) - sizeof(to_rep)) * 8));
                        } else {
                            to_rep s = static_cast<to_rep>(r);
                            s |= s << (sizeof(from_rep) * 8);
                            if constexpr (sizeof(to_rep) > sizeof(from_rep) * 2) {
                                s |= s << (sizeof(from_rep) * 8 * 2);
                                if constexpr (sizeof(to_rep) > sizeof(from_rep) * 4) {
                                    s |= s << (sizeof(from_rep) * 8 * 4);
                                }
                            }
                            return s;
                        }
                    }
                    return static_cast<to_rep>(r);
                }

                template <typename format, typename to_rep, typename from_rep, size_t... indexes>
                constexpr color<format, to_rep> cast_rep(const color<format, from_rep> &c, const std::index_sequence<indexes...> &indexes_capture) noexcept {
                    return color<format, to_rep>(cast_rep_single<decltype(std::get<indexes>(std::declval<color<format, to_rep>>()))>(std::get<indexes>(c))...);
                }

                template <typename rep>
                using floating_point_noref = std::conditional<
                    std::is_arithmetic<rep>::value,
                    typename std::conditional<
                        std::is_integral<rep>::value,
                        typename std::conditional<
                            std::is_same<rep, uint64_t>::value,
                            double,
                            float
                        >::type,
                        rep
                    >::type,
                    long double
                >;

                template <typename rep>
                struct floating_point {
                    using type = typename floating_point_noref<typename std::remove_reference<rep>::type>::type;

                    template <typename from>
                    static constexpr type cast(const from &f) noexcept {
                        if constexpr (std::is_integral<from>::value) {
                            return static_cast<type>(f) / static_cast<type>(std::numeric_limits<from>::max());
                        } else {
                            return static_cast<type>(f);
                        }
                    }
                };

                template <typename format, typename rep, size_t... indexes>
                constexpr floating_point<typename std::common_type<typename std::tuple_element<indexes, color<format, rep>>::type...>::type> common_floating_point_impl(const std::index_sequence<indexes...> &indexes_capture) noexcept;

                template <typename format, typename rep>
                struct common_floating_point : decltype(common_floating_point_impl<format, rep>(std::make_index_sequence<std::tuple_size<color<format, rep>>::value>())) {
                };
            }
        }

        template <typename format, typename rep>
        constexpr color<format, rep> color_cast(const color<format, rep> &c) noexcept {
            return c;
        }

        template <typename format, typename to_rep, typename from_rep, typename enable = typename std::enable_if<!std::is_same<to_rep, from_rep>::value>::type>
        constexpr color<format, to_rep> color_cast(const color<format, from_rep> &c) noexcept {
            return detail::color_cast::cast_rep<format, to_rep>(c, std::make_index_sequence<std::tuple_size<color<format, from_rep>>::value>());
        }

        template <typename to_format, typename rep, typename enable = typename std::enable_if<std::is_same<to_format, hsv>::value>::type>
        constexpr color<to_format, rep> color_cast(const color<rgb, rep> &c) noexcept {
            // https://stackoverflow.com/a/6930407
            using floating_point = detail::color_cast::common_floating_point<rgb, rep>;
            typename floating_point::type red = floating_point::cast(c.red());
            typename floating_point::type green = floating_point::cast(c.green());
            typename floating_point::type blue = floating_point::cast(c.blue());
            typename floating_point::type lowest = std::min(red, std::min(green, blue));
            typename floating_point::type highest = std::max(red, std::max(green, blue));
            typename floating_point::type delta = highest - lowest;
            if (delta == 0) {
                return color_cast<hsv, rep>(color<hsv, typename floating_point::type>(0, 0, highest));
            }
            typename floating_point::type hue;
            if (red >= highest) {
                hue = (green - blue) / delta;
                if (hue < 0) {
                    hue += 6;
                }
            } else if (green >= highest) {
                hue = 2 + (blue - red) / delta;
            } else {
                hue = 4 + (red - green) / delta;
            }
            hue /= 6;
            return color_cast<hsv, rep>(color<hsv, typename floating_point::type>(hue / 6, delta / highest, highest));
        }

        template <typename to_format, typename rep, typename enable = typename std::enable_if<std::is_same<to_format, rgb>::value>::type>
        constexpr color<to_format, rep> color_cast(const color<hsv, rep> &c) noexcept {
            // https://stackoverflow.com/a/6930407
            using floating_point = detail::color_cast::common_floating_point<hsv, rep>;
            typename floating_point::type hue = floating_point::cast(c.hue());
            typename floating_point::type saturation = floating_point::cast(c.saturation());
            typename floating_point::type value = floating_point::cast(c.value());
            typename floating_point::type one = 1;
            typename floating_point::type hue_sector_width = one / 6;
            unsigned hue_sector = static_cast<unsigned>(hue / hue_sector_width);
            typename floating_point::type hue_pos = (hue - hue_sector * hue_sector_width) / hue_sector_width;
            typename floating_point::type bright = value;
            typename floating_point::type dark = value * (one - saturation);
            typename floating_point::type ramping_down = value * (one - (saturation * hue_pos));
            typename floating_point::type ramping_up = value * (one - (saturation * (one - hue_pos)));
            switch (hue_sector) {
                case 0:
                    return color_cast<rgb, rep>(color<rgb, typename floating_point::type>(bright, ramping_up, dark));
                case 1:
                    return color_cast<rgb, rep>(color<rgb, typename floating_point::type>(ramping_down, bright, dark));
                case 2:
                    return color_cast<rgb, rep>(color<rgb, typename floating_point::type>(dark, bright, ramping_up));
                case 3:
                    return color_cast<rgb, rep>(color<rgb, typename floating_point::type>(dark, ramping_down, bright));
                case 4:
                    return color_cast<rgb, rep>(color<rgb, typename floating_point::type>(ramping_up, dark, bright));
                case 5:
                    return color_cast<rgb, rep>(color<rgb, typename floating_point::type>(bright, dark, ramping_down));
                default:
                    return color_cast<rgb, rep>(color<rgb, typename floating_point::type>(bright, dark, dark));
            }
        }

        template <typename to_format, typename to_rep, typename from_format, typename from_rep, typename enable = typename std::enable_if<!std::is_same<to_format, from_format>::value && !std::is_same<to_rep, from_rep>::value>::type>
        constexpr color<to_format, to_rep> color_cast(const color<from_format, from_rep> &c) noexcept {
            using floating_point = detail::color_cast::common_floating_point<from_format, from_rep>;
            return color_cast<to_format, to_rep>(color_cast<to_format, typename floating_point::type>(color_cast<from_format, typename floating_point::type>(c)));
        }
    }
}

#endif
