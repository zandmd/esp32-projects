#ifndef ZANDMD_DRIVERS_LORA_HPP
#define ZANDMD_DRIVERS_LORA_HPP

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <zandmd/bus/spidev.hpp>
#include <zandmd/drivers/gpio_isr_service.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

namespace zandmd {
    namespace drivers {
        class lora : private gpio_isr_service {
            public:
                static constexpr size_t MAX_PACKET_LENGTH = 0xFF - sizeof(uint32_t);

                explicit lora(bus::spidev &spi, peripheral_alloc::generic_gpio reset, peripheral_alloc::generic_gpio irq, const char *ssid) noexcept;
                ~lora() noexcept;

                void enable() noexcept;
                void disable() noexcept;

                void get_debug(int &rssi, float &snr) noexcept;

                template <typename Type>
                inline lora &operator <<(const Type &obj) noexcept {
                    spi << send_pre(sizeof(obj)) << obj;
                    send_post();
                    return *this;
                }

                template <typename Type>
                inline lora &operator >>(Type &obj) noexcept {
                    uint8_t op = recv_pre(sizeof(obj));
                    if (op == 0xFF) {
                        memset(&obj, 0, sizeof(obj));
                    } else {
                        spi << op >> obj;
                    }
                    recv_post();
                    return *this;
                }

            private:
                uint8_t send_pre(size_t size) noexcept;
                void send_post() noexcept;
                uint8_t recv_pre(size_t size) noexcept;
                void recv_post() noexcept;
                void isr() noexcept override;

                bus::spidev &spi;
                peripheral_alloc::generic_gpio reset;
                peripheral_alloc::generic_gpio irq;
                StaticSemaphore_t sem_mem;
                SemaphoreHandle_t sem;
                TaskHandle_t task;
                bool enabled;
                uint32_t ssid;
        };
    }
}

#endif
