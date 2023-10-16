#ifndef ZANDMD_BUS_SPI_HPP
#define ZANDMD_BUS_SPI_HPP

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <hal/spi_types.h>
#include <zandmd/bus/spidev.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>
#include <zandmd/peripheral-alloc/gpio.hpp>

namespace zandmd {
    namespace bus {
        class spi {
            public:
                class builder {
                    friend class spi;

                    public:
                        operator spidev() noexcept;

                        inline void freq(int hz) noexcept {
                            config.clock_speed_hz = hz;
                        }

                        inline void mode0() noexcept {
                            config.mode = 0;
                        }

                        inline void mode1() noexcept {
                            config.mode = 1;
                        }

                        inline void mode2() noexcept {
                            config.mode = 2;
                        }

                        inline void mode3() noexcept {
                            config.mode = 3;
                        }

                    private:
                        builder(spi_host_device_t dev, peripheral_alloc::generic_gpio cs) noexcept;

                        spi_host_device_t dev;
                        spi_device_interface_config_t config;
                };

                inline spi(peripheral_alloc::gpio<GPIO_NUM_23> mosi, peripheral_alloc::gpio<GPIO_NUM_19> miso, peripheral_alloc::gpio<GPIO_NUM_18> sclk) noexcept
                        : spi(VSPI_HOST, mosi, miso, sclk) {
                }

                inline spi(peripheral_alloc::gpio<GPIO_NUM_13> mosi, peripheral_alloc::gpio<GPIO_NUM_12> miso, peripheral_alloc::gpio<GPIO_NUM_14> sclk) noexcept
                        : spi(HSPI_HOST, mosi, miso, sclk) {
                }

                ~spi() noexcept;

                builder operator [](peripheral_alloc::generic_gpio cs) noexcept;

            private:
                spi(spi_host_device_t dev, peripheral_alloc::generic_gpio mosi, peripheral_alloc::generic_gpio miso, peripheral_alloc::generic_gpio sclk) noexcept;

                spi_host_device_t dev;
        };
    }
}

#endif
