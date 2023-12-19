#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/button.hpp>

using namespace zandmd::bsp;
using namespace zandmd::drivers;

button peripherals::btn(gpio::btn);
