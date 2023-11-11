#ifndef ZANDMD_BSP_TASKS_HPP
#define ZANDMD_BSP_TASKS_HPP

namespace zandmd {
    namespace bsp {
        class tasks {
            public:
                static constexpr int ledtask = 1;
                static constexpr int buttontask = 2;
                static constexpr int chargetask = 3;
                static constexpr int maintask = 4;

            private:
        };
    }
}

#endif
