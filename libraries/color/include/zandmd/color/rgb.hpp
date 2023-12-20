#ifndef ZANDMD_COLOR_RGB_HPP
#define ZANDMD_COLOR_RGB_HPP

#include <tuple>
#include <type_traits>
#include <utility>
#include <zandmd/color/color.hpp>
#include <zandmd/color/memory.hpp>

namespace zandmd {
    namespace color {
        class rgb {
            private:
                rgb() noexcept;
        };

        template <typename rep_>
        class color<rgb, rep_> {
            public:
                using format = rgb;
                using rep = rep_;
                using red_type = typename std::remove_reference<decltype(std::declval<memory<rep, 3>>().template get<0>())>::type;
                using green_type = typename std::remove_reference<decltype(std::declval<memory<rep, 3>>().template get<1>())>::type;
                using blue_type = typename std::remove_reference<decltype(std::declval<memory<rep, 3>>().template get<2>())>::type;

                constexpr color(red_type red, green_type green, blue_type blue) noexcept : mem(red, green, blue) {
                }

                constexpr color() noexcept : mem(0, 0, 0) {
                }

                constexpr bool operator ==(const color &other) const noexcept {
                    return red() == other.red() && green() == other.green() && blue() == other.blue();
                }

                constexpr bool operator !=(const color &other) const noexcept {
                    return red() != other.red() || green() != other.green() || blue() != other.blue();
                }

                constexpr red_type &red() noexcept {
                    return mem.template get<0>();
                }

                constexpr red_type red() const noexcept {
                    return mem.template get<0>();
                }

                constexpr green_type &green() noexcept {
                    return mem.template get<1>();
                }

                constexpr green_type green() const noexcept {
                    return mem.template get<1>();
                }

                constexpr blue_type &blue() noexcept {
                    return mem.template get<2>();
                }

                constexpr blue_type blue() const noexcept {
                    return mem.template get<2>();
                }

            private:
                memory<rep, 3> mem;
        };
    }
}

namespace std {
    template <typename rep>
    struct tuple_element<0, zandmd::color::color<zandmd::color::rgb, rep>> {
        using type = typename zandmd::color::color<zandmd::color::rgb, rep>::red_type;

        static constexpr type &get(zandmd::color::color<zandmd::color::rgb, rep> &c) noexcept {
            return c.red();
        }

        static constexpr type get(const zandmd::color::color<zandmd::color::rgb, rep> &c) noexcept {
            return c.red();
        }
    };

    template <typename rep>
    struct tuple_element<1, zandmd::color::color<zandmd::color::rgb, rep>> {
        using type = typename zandmd::color::color<zandmd::color::rgb, rep>::green_type;

        static constexpr type &get(zandmd::color::color<zandmd::color::rgb, rep> &c) noexcept {
            return c.green();
        }

        static constexpr type get(const zandmd::color::color<zandmd::color::rgb, rep> &c) noexcept {
            return c.green();
        }
    };

    template <typename rep>
    struct tuple_element<2, zandmd::color::color<zandmd::color::rgb, rep>> {
        using type = typename zandmd::color::color<zandmd::color::rgb, rep>::blue_type;

        static constexpr type &get(zandmd::color::color<zandmd::color::rgb, rep> &c) noexcept {
            return c.blue();
        }

        static constexpr type get(const zandmd::color::color<zandmd::color::rgb, rep> &c) noexcept {
            return c.blue();
        }
    };

    template <typename rep>
    struct tuple_size<zandmd::color::color<zandmd::color::rgb, rep>> : integral_constant<size_t, 3> {
    };
}

#endif
