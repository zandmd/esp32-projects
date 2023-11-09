#include <chrono>
#include <zandmd/bsp/bus.hpp>
#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/button.hpp>
#include <zandmd/drivers/lsm6dso.hpp>
#include <zandmd/drivers/power_dump.hpp>
#include <zandmd/drivers/ws2811.hpp>

using namespace std::chrono;
using namespace zandmd::bsp;
using namespace zandmd::drivers;

button peripherals::button(gpio::btn);
ws2811 peripherals::led(gpio::led);
power_dump peripherals::screwdriver(gpio::out1, seconds(1));
lsm6dso peripherals::imu(bus::imu, gpio::int1);
