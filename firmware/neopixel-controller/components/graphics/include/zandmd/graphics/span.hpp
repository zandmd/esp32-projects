#ifndef ZANDMD_GRAPHICS_SPAN_HPP
#define ZANDMD_GRAPHICS_SPAN_HPP

#include <stddef.h>
#include <zandmd/graphics/multi_span.hpp>

namespace zandmd {
    namespace graphics {
        class span : public multi_span {
            public:
                template <typename type, ptrdiff_t step>
                class generic_iterator : public multi_span::generic_iterator<type, step> {
                    friend class span;

                    public:
                        constexpr generic_iterator() noexcept {
                        }

                        constexpr operator generic_iterator<const type, step>() const noexcept {
                            return generic_iterator<const type, step>(static_cast<multi_span::generic_iterator<const type, step>>(*this));
                        }

                        constexpr operator typename multi_span::generic_iterator<type, step>::pointer() noexcept {
                            return &**this;
                        }

                        constexpr operator typename multi_span::generic_iterator<type, step>::const_pointer() const noexcept {
                            return &**this;
                        }

                        constexpr generic_iterator &operator ++() noexcept {
                            multi_span::generic_iterator<type, step>::operator ++();
                            return *this;
                        }

                        constexpr generic_iterator &operator --() noexcept {
                            multi_span::generic_iterator<type, step>::operator --();
                            return *this;
                        }

                        constexpr generic_iterator operator +(difference_type diff) const noexcept {
                            return multi_span::generic_iterator<type, step>::operator +(diff);
                        }

                        constexpr generic_iterator &operator +=(difference_type diff) noexcept {
                            multi_span::generic_iterator<type, step>::operator +=(diff);
                            return *this;
                        }

                        constexpr generic_iterator operator -(difference_type diff) const noexcept {
                            return multi_span::generic_iterator<type, step>::operator -(diff);
                        }

                        constexpr generic_iterator &operator -=(difference_type diff) noexcept {
                            multi_span::generic_iterator<type, step>::operator -=(diff);
                            return *this;
                        }

                    private:
                        constexpr generic_iterator(const multi_span::generic_iterator<type, step> &copy) noexcept : multi_span::generic_iterator<type, step>(copy) {
                        }
                };

                using iterator = generic_iterator<value_type, 1>;
                using const_iterator = generic_iterator<const value_type, 1>;

                constexpr span(pointer begin, pointer end) noexcept : multi_span(&value, &value), value(begin, end), terminator(nullptr, nullptr) {
                }

                constexpr span(pointer begin, size_type size) noexcept : span(begin, begin + size) {
                }

                constexpr iterator begin() noexcept {
                    return multi_span::begin();
                }

                constexpr const_iterator begin() const noexcept {
                    return multi_span::begin();
                }

                constexpr iterator end() noexcept {
                    return multi_span::end();
                }

                constexpr const_iterator end() const noexcept {
                    return multi_span::end();
                }

                constexpr const_iterator cbegin() const noexcept {
                    return multi_span::cbegin();
                }

                constexpr const_iterator cend() const noexcept {
                    return multi_span::cend();
                }

            private:
                std::tuple<pointer, pointer> value;
                std::tuple<pointer, pointer> terminator;
        };
    }
}

template <typename type, ptrdiff_t step>
constexpr zandmd::graphics::span::generic_iterator<type, step> operator +(ptrdiff_t diff, const zandmd::graphics::span::generic_iterator<type, step> &it) noexcept {
    return it + diff;
}

#endif
