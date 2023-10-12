#define ZANDMD_PERIPHERAL_ALLOC_SRC_GPIO_CPP

#define ZANDMD_GPIO(name, pin) \
    static bool used_gpio_ ## pin = true

#pragma GCC diagnostic ignored "-Wunused-variable"
#include <zandmd/bsp/gpio.hpp>

#ifdef ZANDMD_GPIO
#error <zandmd/bsp/gpio.hpp> should include <zandmd/peripheral-alloc/gpio.hpp> twice
#endif
