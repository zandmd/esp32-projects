#ifndef ZANDMD_BSP_GPIO_HPP
#define ZANDMD_BSP_GPIO_HPP

#include <zandmd/peripheral-alloc/gpio_map.hpp>

ZANDMD_GPIO(io0, 0);
ZANDMD_GPIO(txd, 1);
ZANDMD_GPIO(int2, 2);
ZANDMD_GPIO(rxd, 3);
ZANDMD_GPIO(adc_5v, 4);
ZANDMD_GPIO(miso, 12);
ZANDMD_GPIO(mosi, 13);
ZANDMD_GPIO(sclk, 14);
ZANDMD_GPIO(cs, 15);
ZANDMD_GPIO(neopixel, 16);
ZANDMD_GPIO(int1, 17);
ZANDMD_GPIO(led_dim, 25);
ZANDMD_GPIO(adc_bat, 32);
ZANDMD_GPIO(led_ctrl, 33);
ZANDMD_GPIO(led_dim, 34);
ZANDMD_GPIO(btn, 36);

#include <zandmd/peripheral-alloc/gpio_map.hpp>

#endif
