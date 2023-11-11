#ifndef ZANDMD_BSP_BUTTON_HPP
#define ZANDMD_BSP_BUTTON_HPP

#include <bitset>
#include <functional>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>


namespace zandmd {
    namespace bsp {
        class button {
            public:
                button() noexcept;
                ~button() noexcept;
                bool get_button_state(int buttonnum) noexcept;
                std::function <void(bool state, int buttonnum)> buttonchange;

            private:
                std::bitset<6> buttonval;
                static void poll_buttons(void * context) noexcept;
                std::bitset<6> lastbuttonval;
                StaticSemaphore_t sem_mem;
                SemaphoreHandle_t sem;

        };
    }
}

#endif
