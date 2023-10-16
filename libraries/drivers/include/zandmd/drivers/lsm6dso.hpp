#ifndef ZANDMD_DRIVERS_LSM6DSO_HPP
#define ZANDMD_DRIVERS_LSM6DSO_HPP

#include <functional>
#include <limits>
#include <stddef.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bus/spidev.hpp>
#include <zandmd/drivers/gpio_isr_service.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

namespace zandmd {
    namespace drivers {
        class lsm6dso : private gpio_isr_service {
            public:
                enum odr {
                    odr_12_5_hz,
                    odr_26_hz,
                    odr_52_hz,
                    odr_104_hz,
                    odr_208_hz,
                    odr_416_hz,
                    odr_833_hz,
                    odr_1660_hz,
                    odr_3330_hz,
                    odr_6660_hz
                };

                enum accel_fs {
                    accel_2_g,
                    accel_4_g,
                    accel_8_g,
                    accel_16_g
                };

                enum gyro_fs {
                    gyro_250_dps,
                    gyro_500_dps,
                    gyro_1000_dps,
                    gyro_2000_dps
                };

                struct sample {
                    class tag_type {
                        public:
                            enum sensor_type {
                                gyro_nc = 0x01,
                                accel_nc,
                                temperature,
                                timestamp,
                                cfg_change,
                                accel_nc_t_2,
                                accel_nc_t_1,
                                accel_2xc,
                                accel_3xc,
                                gyro_nc_t_2,
                                gyro_nc_t_1,
                                gyro_2xc,
                                gyro_3xc,
                                sensor_hub_0,
                                sensor_hub_1,
                                sensor_hub_2,
                                sensor_hub_3,
                                step_counter,
                                sensor_hub_nack = 0x19
                            };

                            constexpr sensor_type sensor() const noexcept {
                                return static_cast<sensor_type>(val >> 3);
                            }

                        private:
                            uint8_t val;
                    };

                    class data_type {
                        public:
                            constexpr int16_t raw_signed() const noexcept {
                                return static_cast<int16_t>(static_cast<uint16_t>(low) | (static_cast<uint16_t>(high) << 8));
                            }

                            constexpr uint16_t raw_unsigned() const noexcept {
                                return static_cast<uint16_t>(low) | (static_cast<uint16_t>(high) << 8);
                            }

                            constexpr float accel(accel_fs fs) const noexcept {
                                return static_cast<float>(raw_signed()) * static_cast<float>(2 << fs) / (static_cast<float>(std::numeric_limits<int16_t>::max()));
                            }

                            constexpr float gyro(gyro_fs fs) const noexcept {
                                return static_cast<float>(raw_signed()) * static_cast<float>(250 << fs) / (static_cast<float>(std::numeric_limits<int16_t>::max()));
                            }

                            constexpr uint16_t timestamp() const noexcept {
                                return raw_unsigned();
                            }

                        private:
                            uint8_t low;
                            uint8_t high;
                    };

                    tag_type tag;
                    data_type x;
                    data_type y;
                    data_type z;
                };
                static_assert(sizeof(sample) == 7, "sample alignment");

                explicit lsm6dso(bus::spidev &spi, peripheral_alloc::generic_gpio int1) noexcept;

                void enable(UBaseType_t priority, odr accel_odr, accel_fs accel_fs, odr gyro_odr, gyro_fs gyro_fs, sample *samples, size_t samples_count) noexcept;
                void disable() noexcept;

                std::function<void(const sample *, size_t)> callback;

            private:
                void isr() noexcept override;
                static void task_func(void *context) noexcept;

                bus::spidev &spi;
                peripheral_alloc::generic_gpio int1;
                StackType_t stack[4096];
                StaticTask_t task_mem;
                TaskHandle_t task;
                sample *samples;
                size_t samples_count;
        };
    }
}

#endif
