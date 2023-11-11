#ifndef ZANDMD_BSP_BUTTON_HPP
#define ZANDMD_BSP_BUTTON_HPP

#include <bitset>
#include <functional>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>


namespace zandmd {
    namespace bsp {
        class button {
            public:
                button() noexcept;
                ~button() noexcept;

                void enable_watchdog() noexcept;

                bool get_button_state(int buttonnum) noexcept;

                std::function <void(bool state, int buttonnum)> buttonchange;

            private:
                std::bitset<6> buttonval;
                static void poll_buttons(void * context) noexcept;
                std::bitset<6> lastbuttonval;
                StaticSemaphore_t sem_mem;
                SemaphoreHandle_t sem;
                StackType_t stack[4096];
                StaticTask_t task_mem;
                TaskHandle_t task;
                bool watchdog;

        };
    }
}

#endif
