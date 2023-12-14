#include <zandmd/bsp/leds.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/drivers/event_loop.hpp>
#include <zandmd/drivers/nonvol.hpp>
#include <zandmd/wifi/station.hpp>

using namespace zandmd;
using namespace zandmd::bsp;
using namespace zandmd::wifi;

drivers::event_loop peripherals::event_loop;
drivers::nonvol peripherals::nonvol;
station peripherals::wifi(event_loop, nonvol);
leds peripherals::lights;
