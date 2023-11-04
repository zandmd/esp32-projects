#ifndef ZANDMD_BSP_BUTTON_HPP
#define ZANDMD_BSP_BUTTON_HPP

#include <bitset>


namespace zandmd {
    namespace bsp {
        class button {
            public:                
                button() noexcept;
                ~button() noexcept;
                bool get_button_state(int buttonnum) noexcept;
                                
            private:
                std::bitset<6> buttonval;
                static void poll_buttons(void * context) noexcept;
        };
    }
}

#endif
