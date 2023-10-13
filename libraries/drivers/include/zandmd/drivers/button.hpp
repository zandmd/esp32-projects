#ifndef ZANDMD_DRIVERS_BUTTON_HPP
#define ZANDMD_DRIVERS_BUTTON_HPP

#include <functional>
#include <stddef.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <zandmd/drivers/gpio_isr_service.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

namespace zandmd {
    namespace drivers {
        class button : private gpio_isr_service {
            public:
                enum event {
                    press,
                    release,
                    click,
                    hold,
                    double_click,
                    triple_click,
                    event_count
                };

                static constexpr size_t event_bits = 3;
                static_assert(event_count <= (1 << event_bits), "event has too many values");

                explicit button(peripheral_alloc::generic_gpio gpio) noexcept;
                ~button() noexcept;

                void enable() noexcept;
                void disable() noexcept;

                std::function<void(event, TickType_t)> callback;

            private:
                void isr() noexcept override;
                static void timer_event(TimerHandle_t timer) noexcept;
                void send_event(event ev, TickType_t timestamp) noexcept;
                static void send_event(void *context, uint32_t data) noexcept;

                peripheral_alloc::generic_gpio gpio;
                TickType_t last_press;
                TickType_t last_release;
                int clicks;
                StaticTimer_t timer_memory;
                TimerHandle_t timer;
        };
    }
}

#endif
