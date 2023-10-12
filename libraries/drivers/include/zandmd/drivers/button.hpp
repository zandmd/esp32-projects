#ifndef ZANDMD_DRIVERS_BUTTON_HPP
#define ZANDMD_DRIVERS_BUTTON_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <zandmd/drivers/gpio_isr_service.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

namespace zandmd {
    namespace drivers {
        class button : private gpio_isr_service {
            public:
                explicit button(peripheral_alloc::generic_gpio gpio) noexcept;
                ~button() noexcept;

                void enable() noexcept;
                void disable() noexcept;

                bool wait(TickType_t timeout = portMAX_DELAY) const noexcept;

            private:
                void isr() noexcept override;

                peripheral_alloc::generic_gpio gpio;
                TickType_t last_up;
                StaticSemaphore_t semaphore_memory;
                SemaphoreHandle_t semaphore;
        };
    }
}

#endif
