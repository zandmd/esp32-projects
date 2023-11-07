#include <zandmd/bsp/button.hpp>
#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/ws2811.hpp>
#include <zandmd/bsp/leds.hpp>

using namespace zandmd::bsp;
using namespace zandmd::drivers;

ws2811 peripherals::led(gpio::led, true);
button peripherals::buttons;
leds peripherals::leds;
