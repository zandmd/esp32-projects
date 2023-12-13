#include <stddef.h>
#include <zandmd/bsp/gpio.hpp>
#include <zandmd/bsp/leds.hpp>
#include <zandmd/drivers/ws2811.hpp>
#include <zandmd/graphics/multi_span.hpp>
#include <zandmd/graphics/span.hpp>

using namespace zandmd::bsp;
using namespace zandmd::drivers;
using namespace zandmd::graphics;

leds::leds() : garage_gutter_l2r(buffer7.begin() + 107, buffer7.end()),
               porch_gutter_l2r(buffer7.begin(), buffer7.begin() + 59),
               sidewalk_b2f(buffer6.begin(), buffer6.end()),
               all({ &garage_gutter_l2r, &porch_gutter_l2r, &sidewalk_b2f }),
               led0(gpio::data0, false, true),
               led1(gpio::data1, false, true),
               led2(gpio::data2, false, true),
               led3(gpio::data3, false, true),
               led4(gpio::data4, false, true),
               led5(gpio::data5, false, true),
               led6(gpio::data6, false, true),
               led7(gpio::data7, false, true) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
    static_assert(sizeof(leds) == offsetof(leds, led0) + sizeof(ws2811) * NUM_STRANDS, "wrong number of led drivers");
#pragma GCC diagnostic pop
    all.clear();
}

void leds::update() noexcept {
    multi_span *spans[] = {
        &buffer0,
        &buffer1,
        &buffer2,
        &buffer3,
        &buffer4,
        &buffer5,
        &buffer6,
        &buffer7
    };
    static_assert(sizeof(spans) / sizeof(spans[0]) == NUM_STRANDS, "wrong number of color buffers");
    ws2811 *to_start = &led0;
    ws2811 *to_wait = to_start;
    ws2811 *end = to_start + NUM_STRANDS;
    size_t start_strand = 0;
    size_t in_progress = 0;
    while (to_wait != end) {
        if (in_progress < CONCURRENT_STRANDS && to_start != end) {
            if (!spans[start_strand]->empty()) {
                span &strand = static_cast<span &>(*spans[start_strand]);
                to_start->start(strand.begin(), strand.size());
                ++in_progress;
            }
            ++to_start;
            ++start_strand;
        } else {
            to_wait++->wait();
            --in_progress;
        }
    }
}
