#ifndef ZANDMD_DRIVERS_GPIO_ISR_SERVICE_HPP
#define ZANDMD_DRIVERS_GPIO_ISR_SERVICE_HPP

#include <hal/gpio_types.h>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

namespace zandmd {
    namespace drivers {
        class gpio_isr_service {
            protected:
                ~gpio_isr_service() noexcept = default;

                void add(peripheral_alloc::generic_gpio gpio, gpio_int_type_t type) noexcept;
                void remove(peripheral_alloc::generic_gpio gpio) noexcept;

            private:
                static void call_isr(void *context) noexcept;

                virtual void isr() noexcept = 0;

                static unsigned count;
        };
    }
}

#endif
