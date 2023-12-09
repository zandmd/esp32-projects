#include <stddef.h>
#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/leds.hpp>
#include <zandmd/drivers/ws2811.hpp>

using namespace zandmd::bsp;
using namespace zandmd::drivers;

leds::leds() : led0(gpio::data0, false, true),
               led1(gpio::data1, false, true),
               led2(gpio::data2, false, true),
               led3(gpio::data3, false, true),
               led4(gpio::data4, false, true),
               led5(gpio::data5, false, true),
               led6(gpio::data6, false, true),
               led7(gpio::data7, false, true) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
    static_assert(sizeof(leds) == offsetof(leds, led0) + sizeof(ws2811) * NUM_STRANDS, "leds should be addressable like an array");
#pragma GCC diagnostic pop
    off();
}

void leds::update() noexcept {
    ws2811 *to_start = &led0;
    ws2811 *to_wait = to_start;
    ws2811 *end = to_start + NUM_STRANDS;
    size_t start_strand = 0;
    size_t in_progress = 0;
    while (to_wait != end) {
        if (in_progress < CONCURRENT_STRANDS && to_start != end) {
            to_start++->start(data[start_strand++].data(), LEDS_PER_STRAND);
            ++in_progress;
        } else {
            to_wait++->wait();
            --in_progress;
        }
    }
}

void leds::off() noexcept {
    for (auto &colors : data) {
        colors.fill(color(0, 0, 0));
    }
}

bool leds::color_wipe(const color &target, size_t counter) noexcept {
    size_t strand = counter / LEDS_PER_STRAND;
    size_t pixel = counter % LEDS_PER_STRAND;
    if (strand >= NUM_STRANDS) {
        return false;
    }
    data[strand][pixel] = target;
    return true;
}
