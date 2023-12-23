#ifndef ZANDMD_GRAPHICS_MULTI_SPAN_HPP
#define ZANDMD_GRAPHICS_MULTI_SPAN_HPP

#include <initializer_list>
#include <iterator>
#include <stddef.h>
#include <stdint.h>
#include <tuple>
#include <type_traits>
#include <zandmd/color/color.hpp>
#include <zandmd/color/hsv.hpp>
#include <zandmd/drivers/ws2811.hpp>
#include <zandmd/math/random.hpp>

namespace zandmd {
    namespace graphics {
        class multi_span {
            public:
                using value_type = drivers::ws2811::color_rgb;
                using reference = value_type &;
                using const_reference = const value_type &;
                using pointer = value_type *;
                using const_pointer = const value_type *;
                using size_type = size_t;
                using difference_type = ptrdiff_t;

                template <typename type, ptrdiff_t step>
                class generic_iterator {
                    friend class multi_span;

                    public:
                        using iterator_category = std::random_access_iterator_tag;
                        using value_type = type;
                        using difference_type = ptrdiff_t;
                        using pointer = value_type *;
                        using const_pointer = const value_type *;
                        using reference = value_type &;
                        using const_reference = const value_type &;

                        constexpr generic_iterator() noexcept : iterators(nullptr), iterator(nullptr) {
                        }

                        constexpr operator generic_iterator<const type, step>() const noexcept {
                            return generic_iterator<const type, step>(iterators, iterator);
                        }

                        constexpr bool operator ==(const generic_iterator &other) const noexcept {
                            return iterators == other.iterators && iterator == other.iterator;
                        }

                        constexpr bool operator !=(const generic_iterator &other) const noexcept {
                            return iterators != other.iterators || iterator != other.iterator;
                        }

                        constexpr bool operator <(const generic_iterator &other) const noexcept {
                            return *this - other < 0;
                        }

                        constexpr bool operator >(const generic_iterator &other) const noexcept {
                            return *this - other > 0;
                        }

                        constexpr bool operator <=(const generic_iterator &other) const noexcept {
                            return *this - other <= 0;
                        }

                        constexpr bool operator >=(const generic_iterator &other) const noexcept {
                            return *this - other >= 0;
                        }

                        constexpr reference operator *() noexcept {
                            return *iterator;
                        }

                        constexpr const_reference operator *() const noexcept {
                            return *iterator;
                        }

                        constexpr pointer operator ->() noexcept {
                            return iterator;
                        }

                        constexpr const_pointer operator ->() const noexcept {
                            return iterator;
                        }

                        constexpr generic_iterator &operator ++() noexcept {
                            return *this += 1;
                        }

                        constexpr generic_iterator &operator --() noexcept {
                            return *this -= 1;
                        }

                        constexpr generic_iterator operator +(difference_type diff) const noexcept {
                            generic_iterator copy = *this;
                            copy += diff;
                            return copy;
                        }

                        constexpr generic_iterator &operator +=(difference_type diff) noexcept {
                            diff *= step;
                            do {
                                iterator += diff;
                                diff = iterator - end();
                                if (diff * step < 0) {
                                    return *this;
                                }
                                iterators += step;
                                iterator = begin();
                            } while (iterator);
                            iterators -= step;
                            iterator = end();
                            return *this;
                        }

                        constexpr generic_iterator operator -(difference_type diff) const noexcept {
                            return *this + -diff;
                        }

                        constexpr difference_type operator -(const generic_iterator &other) const noexcept {
                            difference_type diff = 0;
                            generic_iterator self = *this;
                            while (self.iterators != other.iterators) {
                                difference_type remaining = self.end() - self.iterator;
                                diff += remaining;
                                self += remaining;
                                if (!self.begin()) {
                                    return -(other - *this);
                                }
                            }
                            return diff + (self.iterator - other.iterator) / step;
                        }

                        constexpr generic_iterator &operator -=(difference_type diff) noexcept {
                            *this += -diff;
                            return *this;
                        }

                        constexpr reference operator [](difference_type n) noexcept {
                            return *(*this + n);
                        }

                        constexpr const_reference operator [](difference_type n) const noexcept {
                            return *(*this + n);
                        }

                    private:
                        using mutable_pointer = typename std::remove_const<value_type>::type *;

                        constexpr generic_iterator(std::tuple<mutable_pointer, mutable_pointer> *iterators, pointer iterator) noexcept : iterators(iterators), iterator(iterator) {
                            if (!iterator) {
                                *this += 0;
                            }
                        }

                        constexpr pointer begin() const noexcept {
                            if constexpr (step > 0) {
                                return std::get<0>(*iterators);
                            } else {
                                return std::get<1>(*iterators) - 1;
                            }
                        }

                        constexpr pointer end() const noexcept {
                            if constexpr (step > 0) {
                                return std::get<1>(*iterators);
                            } else {
                                return std::get<0>(*iterators) - 1;
                            }
                        }

                        std::tuple<mutable_pointer, mutable_pointer> *iterators;
                        pointer iterator;
                };

                enum origin {
                    start,
                    center,
                    finish
                };

                using iterator = generic_iterator<value_type, 1>;
                using const_iterator = generic_iterator<const value_type, 1>;
                using reverse_iterator = generic_iterator<value_type, -1>;
                using const_reverse_iterator = generic_iterator<const value_type, -1>;

                constexpr iterator begin() noexcept {
                    return iterator(first, std::get<0>(*first));
                }

                constexpr const_iterator begin() const noexcept {
                    return const_iterator(first, std::get<0>(*first));
                }

                constexpr iterator end() noexcept {
                    return iterator(last, std::get<1>(*last));
                }

                constexpr const_iterator end() const noexcept {
                    return const_iterator(last, std::get<1>(*last));
                }

                constexpr reverse_iterator rbegin() noexcept {
                    return reverse_iterator(last, std::get<1>(*last) - 1);
                }

                constexpr const_reverse_iterator rbegin() const noexcept {
                    return const_reverse_iterator(last, std::get<1>(*last) - 1);
                }

                constexpr reverse_iterator rend() noexcept {
                    return reverse_iterator(first, std::get<0>(*first) - 1);
                }

                constexpr const_reverse_iterator rend() const noexcept {
                    return const_reverse_iterator(first, std::get<0>(*first) - 1);
                }

                constexpr const_iterator cbegin() const noexcept {
                    return const_iterator(first, std::get<0>(*first));
                }

                constexpr const_iterator cend() const noexcept {
                    return const_iterator(last, std::get<1>(*last));
                }

                constexpr const_reverse_iterator crbegin() const noexcept {
                    return const_reverse_iterator(last, std::get<1>(*last) - 1);
                }

                constexpr const_reverse_iterator crend() const noexcept {
                    return const_reverse_iterator(first, std::get<0>(*first) - 1);
                }

                constexpr size_type size() const noexcept {
                    size_type sz = 0;
                    for (auto it = first; ; ++it) {
                        sz += std::get<1>(*it) - std::get<0>(*it);
                        if (it == last) {
                            return sz;
                        }
                    }
                }

                constexpr size_type max_size() const noexcept {
                    return size();
                }

                constexpr bool empty() const noexcept {
                    return !std::get<0>(*first);
                }

                constexpr reference operator [](size_type n) noexcept {
                    return *(begin() + n);
                }

                constexpr const_reference operator [](size_type n) const noexcept {
                    return *(begin() + n);
                }

                constexpr reference front() noexcept {
                    return std::get<0>(*first)[0];
                }

                constexpr const_reference front() const noexcept {
                    return std::get<0>(*first)[0];
                }

                constexpr reference back() noexcept {
                    return std::get<1>(*last)[-1];
                }

                constexpr const_reference back() const noexcept {
                    return std::get<1>(*last)[-1];
                }

                void clear() noexcept;
                void fill(const value_type &val) noexcept;
                bool color_wipe(const value_type &val, size_type counter, origin origin = start) noexcept;
                void stripe(const std::initializer_list<value_type> &sequence, size_type counter) noexcept;
                bool random(const value_type &val, math::random &rand) noexcept;
                void rainbow(const color::color<color::hsv, uint8_t> &val, size_type counter) noexcept;
                void rainbow_symmetric(const color::color<color::hsv, uint8_t> &val, uint8_t hue_min, uint8_t hue_max, size_type turnaround, size_type counter) noexcept;

            protected:
                constexpr multi_span(std::tuple<pointer, pointer> *first, std::tuple<pointer, pointer> *last) noexcept : first(first), last(last) {
                }

                constexpr multi_span(std::tuple<pointer, pointer> *first, size_type count) noexcept : multi_span(first, first + count - 1) {
                }

            private:
                mutable std::tuple<pointer, pointer> *first;
                mutable std::tuple<pointer, pointer> *last;
        };
    }
}

template <typename type, ptrdiff_t step>
constexpr zandmd::graphics::multi_span::generic_iterator<type, step> operator +(ptrdiff_t diff, const zandmd::graphics::multi_span::generic_iterator<type, step> &it) noexcept {
    return it + diff;
}

#endif
