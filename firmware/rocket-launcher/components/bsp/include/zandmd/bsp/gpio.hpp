#ifndef ZANDMD_BSP_GPIO_HPP
#define ZANDMD_BSP_GPIO_HPP

#include <zandmd/peripheral-alloc/gpio_map.hpp>

ZANDMD_GPIO(btn_c0, 34);
ZANDMD_GPIO(btn_c1, 35);
ZANDMD_GPIO(btn_r0, 21);
ZANDMD_GPIO(btn_r1, 32);
ZANDMD_GPIO(btn_r2, 33);
ZANDMD_GPIO(cs, 5);
ZANDMD_GPIO(fire0, 26);
ZANDMD_GPIO(fire1, 27);
ZANDMD_GPIO(fire2, 16);
ZANDMD_GPIO(fire3, 17);
ZANDMD_GPIO(led, 14);
ZANDMD_GPIO(miso, 19);
ZANDMD_GPIO(mosi, 23);
ZANDMD_GPIO(rfm_irq, 15);
ZANDMD_GPIO(rfm_rst,  22);
ZANDMD_GPIO(sclk, 18);
ZANDMD_GPIO(sense0, 25);
ZANDMD_GPIO(sense1, 12);
ZANDMD_GPIO(sense2, 2);
ZANDMD_GPIO(sense3, 4);
ZANDMD_GPIO(vbat, 13);

#include <zandmd/peripheral-alloc/gpio_map.hpp>

#endif
