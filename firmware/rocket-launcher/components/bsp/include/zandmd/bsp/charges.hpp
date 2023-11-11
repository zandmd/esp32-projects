#ifndef ZANDMD_BSP_CHARGES_HPP
#define ZANDMD_BSP_CHARGES_HPP

#include <array>
#include <bitset>
#include <chrono>
#include <functional>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <zandmd/drivers/adc_service.hpp>
#include <zandmd/drivers/power_dump.hpp>
#include <zandmd/peripheral-alloc/generic_adc.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

namespace zandmd {
    namespace bsp {
        class charges {
            public:
                using mask = std::bitset<4>;

                explicit charges(UBaseType_t priority, const std::array<peripheral_alloc::generic_adc, 4> &senses, const std::array<peripheral_alloc::generic_gpio, 4> &fires, std::chrono::milliseconds duration = std::chrono::milliseconds(100)) noexcept;
                ~charges() noexcept;

                mask check() const noexcept;

                bool arm(mask charges) noexcept;
                bool disarm(mask charges) noexcept;
                void disarm_all() noexcept;
                mask are_armed() const noexcept;

                bool fire(mask charges) noexcept;

                std::function<void(mask new_state, mask old_state)> charge_event;

            private:
                static void task_func(void *context) noexcept;

                drivers::adc_service senses;
                drivers::adc_service senses1;
                drivers::adc_service senses2;
                drivers::adc_service senses3;
                drivers::power_dump fires;
                drivers::power_dump fires1;
                drivers::power_dump fires2;
                drivers::power_dump fires3;
                StaticSemaphore_t sem_mem;
                SemaphoreHandle_t sem;
                StackType_t stack[4096];
                StaticTask_t task_mem;
                TaskHandle_t task;
                mask continuity;
        };
    }
}

#endif
