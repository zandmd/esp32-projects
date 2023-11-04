#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/button.hpp>
#include <zandmd/drivers/ws2811.hpp>

using namespace zandmd::bsp;
using namespace zandmd::drivers;

button peripherals::button(gpio::btn);
ws2811 peripherals::led(gpio::led);
