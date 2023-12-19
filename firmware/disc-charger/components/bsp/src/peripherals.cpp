#include <zandmd/bsp/analog.hpp>
#include <zandmd/bsp/bus.hpp>
#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/leds.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/button.hpp>
#include <zandmd/drivers/lsm6dso.hpp>
#include <zandmd/drivers/ws2811.hpp>

using namespace zandmd::bsp;
using namespace zandmd::drivers;

button peripherals::btn(gpio::btn);
ws2811 peripherals::status_led(gpio::neopixel);
lsm6dso peripherals::imu(bus::imu, gpio::int1);
analog peripherals::adc;
leds peripherals::uv;
