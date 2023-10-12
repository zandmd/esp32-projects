#ifndef ZANDMD_DRIVERS_WS2811_HDPP
#define ZANDMD_DRIVERS_WS2811_HDPP

#include <stddef.h>
#include <stdint.h>
#include <type_traits>
#include <driver/rmt_encoder.h>
#include <driver/rmt_tx.h>
#include <driver/rmt_types.h>
#include <esp_err.h>
#include <zandmd/color/color.hpp>
#include <zandmd/color/memory.hpp>
#include <zandmd/color/rgb.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

namespace zandmd {
    namespace drivers {
        class ws2811 : private rmt_encoder_t {
            public:
                using color = zandmd::color::color<zandmd::color::rgb, ws2811>;

                explicit ws2811(peripheral_alloc::generic_gpio gpio) noexcept;
                ~ws2811() noexcept;

                void start(const color *colors, size_t size) noexcept;
                void wait() const noexcept;

            private:
                static size_t encode(rmt_encoder_t *encoder, rmt_channel_handle_t chan, const void *data, size_t size, rmt_encode_state_t *state) noexcept;
                static esp_err_t reset(rmt_encoder_t *encoder) noexcept;
                static esp_err_t del(rmt_encoder_t *encoder) noexcept;

                rmt_channel_handle_t chan;
                rmt_encoder_handle_t bytes_encoder;
                rmt_encoder_handle_t copy_encoder;
                rmt_encode_state_t state;
        };
    }

    namespace color {
        template <>
        class memory<drivers::ws2811, 3> {
            public:
                constexpr memory(uint8_t r, uint8_t g, uint8_t b) : g(g), r(r), b(b) {
                }

                template <size_t n>
                constexpr uint8_t &get() noexcept {
                    if constexpr (n == 0) {
                        return r;
                    } else if constexpr (n == 1) {
                        return g;
                    } else {
                        return b;
                    }
                }

                template <size_t n>
                constexpr uint8_t get() const noexcept {
                    if constexpr (n == 0) {
                        return r;
                    } else if constexpr (n == 1) {
                        return g;
                    } else {
                        return b;
                    }
                }

            private:
                uint8_t g;
                uint8_t r;
                uint8_t b;
        };
        static_assert(sizeof(memory<drivers::ws2811, 3>) == 3, "memory<drivers::ws2811> alignment");
    }
}

#endif
