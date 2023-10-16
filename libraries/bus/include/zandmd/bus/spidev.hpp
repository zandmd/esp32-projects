#ifndef ZANDMD_BUS_SPIDEV_HPP
#define ZANDMD_BUS_SPIDEV_HPP

#include <algorithm>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <tuple>
#include <type_traits>
#include <utility>
#include <driver/spi_master.h>
#include <zandmd/bus/duplex.hpp>

namespace zandmd {
    namespace bus {
        class spi;

        class spidev {
            friend class spi;

            public:
                template <bool is_full_duplex, typename rx_tuple, typename tx_tuple>
                class transaction {
                    friend class spidev;

                    public:
                        inline ~transaction() noexcept {
                            if (send) {
                                send = false;
                                uint8_t buffer[is_full_duplex ? std::max(tuple_size<rx_tuple>(), tuple_size<tx_tuple>()) : tuple_size<rx_tuple>()];
                                dev.transfer(buffer, sizeof(buffer), &transaction::rx_func, &transaction::tx_func, this);
                            }
                        }

                        template <typename type, typename enable = typename std::enable_if<(sizeof(type) > 0)>::type>
                        constexpr auto operator >>(type &val) noexcept {
                            auto new_rx = std::tuple_cat(rx, std::tuple<type *>(&val));
                            auto new_tx = append<type, tx_tuple>(tx);
                            send = false;
                            return transaction<is_full_duplex, decltype(new_rx), decltype(new_tx)>(dev, new_rx, new_tx);
                        }

                        template <typename type, typename enable = typename std::enable_if<(sizeof(type) > 0)>::type>
                        constexpr auto operator <<(const type &val) noexcept {
                            auto new_rx = append<type, rx_tuple>(rx);
                            auto new_tx = std::tuple_cat(tx, std::tuple<const type *>(&val));
                            send = false;
                            return transaction<is_full_duplex, decltype(new_rx), decltype(new_tx)>(dev, new_rx, new_tx);
                        }

                    private:
                        constexpr transaction(spidev &dev, rx_tuple rx = rx_tuple(), tx_tuple tx = tx_tuple()) noexcept : dev(dev), rx(rx), tx(tx), send(true) {
                        }

                        template <typename type, typename tuple>
                        static constexpr auto append(typename std::enable_if<!is_full_duplex, tuple>::type in) noexcept {
                            return std::tuple_cat(in, std::tuple<type *>(nullptr));
                        }

                        template <typename type, typename tuple>
                        static constexpr auto append(typename std::enable_if<is_full_duplex, tuple>::type in) noexcept {
                            return in;
                        }

                        template <size_t n = 0>
                        inline void rx_func_loop(const uint8_t *buffer) noexcept {
                            if constexpr (n < std::tuple_size<rx_tuple>::value) {
                                auto v = std::get<n>(rx);
                                if (v) {
                                    memcpy(v, buffer, sizeof(*v));
                                }
                                rx_func_loop<n + 1>(buffer + sizeof(*v));
                            }
                        }

                        static void rx_func(const void *buffer, void *context) noexcept {
                            static_cast<transaction *>(context)->rx_func_loop<>(reinterpret_cast<const uint8_t *>(buffer));
                        }

                        template <size_t n = 0>
                        inline void tx_func_loop(uint8_t *buffer) noexcept {
                            if constexpr (n < std::tuple_size<tx_tuple>::value) {
                                auto v = std::get<n>(tx);
                                if (v) {
                                    memcpy(buffer, v, sizeof(*v));
                                }
                                tx_func_loop<n + 1>(buffer + sizeof(*v));
                            }
                        }

                        static void tx_func(void *buffer, void *context) noexcept {
                            static_cast<transaction *>(context)->tx_func_loop<>(reinterpret_cast<uint8_t *>(buffer));
                        }

                        template <typename tuple, size_t n = 0>
                        static constexpr size_t tuple_size() noexcept {
                            if constexpr (n < std::tuple_size<tuple>::value) {
                                return sizeof(*std::get<n>(std::declval<tuple>())) + tuple_size<tuple, n + 1>();
                            } else {
                                return 0;
                            }
                        }

                        spidev &dev;
                        rx_tuple rx;
                        tx_tuple tx;
                        bool send;
                };

                ~spidev() noexcept;

                inline auto operator >>(const full_duplex_type &) noexcept {
                    return transaction<true, std::tuple<>, std::tuple<>>(*this);
                }

                inline auto operator <<(const full_duplex_type &) noexcept {
                    return transaction<true, std::tuple<>, std::tuple<>>(*this);
                }

                inline auto operator >>(const half_duplex_type &) noexcept {
                    return transaction<false, std::tuple<>, std::tuple<>>(*this);
                }

                inline auto operator <<(const half_duplex_type &) noexcept {
                    return transaction<false, std::tuple<>, std::tuple<>>(*this);
                }

                template <typename type>
                inline auto operator >>(type &val) noexcept {
                    return *this >> half_duplex >> val;
                }

                template <typename type>
                inline auto operator <<(const type &val) noexcept {
                    return *this << half_duplex << val;
                }

            private:
                spidev(spi_device_handle_t handle) noexcept;

                void transfer(void *buffer, size_t size, void (*rx_func)(const void *buffer, void *context), void (*tx_func)(void *buffer, void *context), void *context) noexcept;

                spi_device_handle_t handle;
        };
    }
}

#include <zandmd/bus/spi.hpp>

#endif
