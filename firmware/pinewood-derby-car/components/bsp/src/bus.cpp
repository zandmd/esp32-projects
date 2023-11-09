#include <zandmd/bsp/bus.hpp>
#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bus/spi.hpp>
#include <zandmd/bus/spidev.hpp>

using namespace zandmd::bsp;
using namespace zandmd::bus;

spi bus::hspi(gpio::mosi, gpio::miso, gpio::sclk);
spidev bus::imu(hspi[gpio::cs]);
