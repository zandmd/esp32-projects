#ifndef ZANDMD_DRIVERS_RTOS_INIT_HPP
#define ZANDMD_DRIVERS_RTOS_INIT_HPP

namespace zandmd {
    namespace drivers {
        class rtos_init {
            public:
                ~rtos_init() noexcept;

                static void post_init() noexcept;

            protected:
                rtos_init() noexcept;

            private:
                virtual void post_rtos_init() noexcept = 0;

                static rtos_init *head;
                static rtos_init **tail;
                rtos_init *next;
        };
    }
}

#endif
