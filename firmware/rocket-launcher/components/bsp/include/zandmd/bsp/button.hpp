#ifndef ZANDMD_BSP_BUTTON_HPP
#define ZANDMD_BSP_BUTTON_HPP

#include <bitset>
#include <functional>


namespace zandmd {
    namespace bsp {
        class button {
            public:                
                button() noexcept;
                bool get_button_state(int buttonnum) noexcept;
                std::function <void(bool state, int buttonnum)> buttonchange;
                                
            private:
                std::bitset<6> buttonval;
                static void poll_buttons(void * context) noexcept;
                std::bitset<6> lastbuttonval;

        };
    }
}

#endif
