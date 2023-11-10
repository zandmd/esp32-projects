#ifndef ZANDMD_DRIVERS_POWER_DUMP_HPP
#define ZANDMD_DRIVERS_POWER_DUMP_HPP

#include <chrono>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

namespace zandmd {
    namespace drivers {
        class power_dump {
            public:
                explicit power_dump(peripheral_alloc::generic_gpio gpio, std::chrono::milliseconds default_duration = std::chrono::milliseconds(100)) noexcept;
                ~power_dump() noexcept;

                bool arm() noexcept;
                bool disarm() noexcept;
                bool is_armed() const noexcept;

                inline bool dump(std::chrono::milliseconds duration = std::chrono::milliseconds(0)) noexcept {
                    power_dump *me = this;
                    return dump_all(&me, &me + 1, duration);
                }

                static bool dump_all(power_dump **begin, power_dump **end, std::chrono::milliseconds duration = std::chrono::milliseconds(0)) noexcept;

            private:
                peripheral_alloc::generic_gpio gpio;
                std::chrono::milliseconds default_duration;
                StaticSemaphore_t semaphore_mem;
                SemaphoreHandle_t semaphore;
        };
    }
}

#endif
