#include <zandmd/bsp/bus.hpp>
#include <zandmd/bsp/button.hpp>
#include <zandmd/bsp/charges.hpp>
#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/bsp/tasks.hpp>
#include <zandmd/drivers/lora.hpp>
#include <zandmd/drivers/ws2811.hpp>
#include <zandmd/bsp/leds.hpp>

using namespace zandmd::bsp;
using namespace zandmd::drivers;

ws2811 peripherals::led(gpio::led, true);
button peripherals::buttons;
leds peripherals::leds;
lora peripherals::lora(bus::rfm, gpio::rfm_rst, gpio::rfm_irq, __FILE__);
charges peripherals::charges(tasks::chargetask, { gpio::sense0, gpio::sense1, gpio::sense2, gpio::sense3 }, { gpio::fire0, gpio::fire1, gpio::fire2, gpio::fire3 });
