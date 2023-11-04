#ifndef ZANDMD_COLOR_HSV_HPP
#define ZANDMD_COLOR_HSV_HPP

#include <tuple>
#include <type_traits>
#include <utility>
#include <zandmd/color/color.hpp>
#include <zandmd/color/memory.hpp>

namespace zandmd {
    namespace color {
        class hsv {
            private:
                hsv() noexcept;
        };

        template <typename rep_>
        class color<hsv, rep_> {
            public:
                using format = hsv;
                using rep = rep_;
                using hue_type = typename std::remove_reference<decltype(std::declval<memory<rep, 3>>().template get<0>())>::type;
                using saturation_type = typename std::remove_reference<decltype(std::declval<memory<rep, 3>>().template get<1>())>::type;
                using value_type = typename std::remove_reference<decltype(std::declval<memory<rep, 3>>().template get<2>())>::type;

                constexpr color(hue_type hue, saturation_type saturation, value_type value) noexcept : mem(hue, saturation, value) {
                }

                constexpr color() noexcept : mem(0, 0, 0) {
                }

                constexpr hue_type &hue() noexcept {
                    return mem.template get<0>();
                }

                constexpr hue_type hue() const noexcept {
                    return mem.template get<0>();
                }

                constexpr saturation_type &saturation() noexcept {
                    return mem.template get<1>();
                }

                constexpr saturation_type saturation() const noexcept {
                    return mem.template get<1>();
                }

                constexpr value_type &value() noexcept {
                    return mem.template get<2>();
                }

                constexpr value_type value() const noexcept {
                    return mem.template get<2>();
                }

            private:
                memory<rep, 3> mem;
        };
    }
}

namespace std {
    template <typename rep>
    struct tuple_element<0, zandmd::color::color<zandmd::color::hsv, rep>> {
        using type = typename zandmd::color::color<zandmd::color::hsv, rep>::hue_type;

        static constexpr type &get(zandmd::color::color<zandmd::color::hsv, rep> &c) noexcept {
            return c.hue();
        }

        static constexpr type get(const zandmd::color::color<zandmd::color::hsv, rep> &c) noexcept {
            return c.hue();
        }
    };

    template <typename rep>
    struct tuple_element<1, zandmd::color::color<zandmd::color::hsv, rep>> {
        using type = typename zandmd::color::color<zandmd::color::hsv, rep>::saturation_type;

        static constexpr type &get(zandmd::color::color<zandmd::color::hsv, rep> &c) noexcept {
            return c.saturation();
        }

        static constexpr type get(const zandmd::color::color<zandmd::color::hsv, rep> &c) noexcept {
            return c.saturation();
        }
    };

    template <typename rep>
    struct tuple_element<2, zandmd::color::color<zandmd::color::hsv, rep>> {
        using type = typename zandmd::color::color<zandmd::color::hsv, rep>::value_type;

        static constexpr type &get(zandmd::color::color<zandmd::color::hsv, rep> &c) noexcept {
            return c.value();
        }

        static constexpr type get(const zandmd::color::color<zandmd::color::hsv, rep> &c) noexcept {
            return c.value();
        }
    };

    template <typename rep>
    struct tuple_size<zandmd::color::color<zandmd::color::hsv, rep>> : integral_constant<size_t, 3> {
    };
}

#endif
