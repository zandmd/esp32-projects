#include <zandmd/bsp/battery.hpp>
#include <zandmd/peripheral-alloc/generic_adc.hpp>

using namespace zandmd::bsp;
using namespace zandmd::peripheral_alloc;

static constexpr int BACKWARDS_THRESH = 10;
static constexpr int LOW_THRESH = 2972; // 11.7 V

battery::battery(generic_adc adc) noexcept : adc(adc) {
}

int battery::poll_raw() const noexcept {
    return adc.poll();
}

battery::state battery::poll() const noexcept {
    int val = poll_raw();
    if (val < BACKWARDS_THRESH) {
        return backwards;
    } else if (val < LOW_THRESH) {
        return low;
    } else {
        return good;
    }
}
