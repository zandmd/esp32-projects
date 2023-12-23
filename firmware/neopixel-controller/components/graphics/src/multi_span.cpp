#include <assert.h>
#include <initializer_list>
#include <stdint.h>
#include <zandmd/color/color.hpp>
#include <zandmd/color/color_cast.hpp>
#include <zandmd/color/hsv.hpp>
#include <zandmd/graphics/multi_span.hpp>
#include <zandmd/math/random.hpp>

using namespace std;
using namespace zandmd::color;
using namespace zandmd::graphics;

void multi_span::clear() noexcept {
    fill(value_type(0, 0, 0));
}

void multi_span::fill(const value_type &val) noexcept {
    for (auto &el : *this) {
        el = val;
    }
}

bool multi_span::color_wipe(const value_type &val, size_type counter, origin origin) noexcept {
    if (counter >= size()) {
        return false;
    }
    switch (origin) {
        case start:
            (*this)[counter] = val;
            break;
        case center:
            (*this)[(size() / 2) + (counter / 2) * (counter % 2 == 0 ? 1 : -1)] = val;
            break;
        case finish:
            (*this)[size() - 1 - counter] = val;
            break;
        default:
            assert(false);
            break;
    }
    return true;
}

void multi_span::stripe(const initializer_list<value_type> &sequence, size_type counter) noexcept {
    counter = sequence.size() - (counter % sequence.size());
    for (auto &el : *this) {
        el = sequence.begin()[counter++ % sequence.size()];
    }
}

bool multi_span::random(const value_type &val, math::random &rand) noexcept {
    size_t total = 0;
    for (const auto &el : *this) {
        if (el != val) {
            ++total;
        }
    }
    if (total == 0) {
        return false;
    }
    total = static_cast<uint32_t>(rand) % total;
    for (auto &el : *this) {
        if (el != val && total-- == 0) {
            el = val;
            return true;
        }
    }
    assert(false);
    return false;
}

void multi_span::rainbow(const color::color<hsv, uint8_t> &val, size_type counter) noexcept {
    color::color<hsv, uint8_t> i = val;
    i.hue() = val.hue() * counter;
    for (auto &el : *this) {
        i.hue() += val.hue();
        el = color_cast<value_type::format, value_type::rep>(i);
    }
}

void multi_span::rainbow_symmetric(const color::color<hsv, uint8_t> &val, uint8_t hue_min, uint8_t hue_max, size_type turnaround, size_type counter) noexcept {
    size_type skew = (hue_max - hue_min) / val.hue() + 1;
    for (auto &el : *this) {
        counter = (counter + 1) % ((skew + turnaround) * 2);
        if (counter < turnaround) {
            el = color_cast<value_type::format, value_type::rep>(color::color<hsv, uint8_t>(hue_min, val.saturation(), val.value()));
        } else if (counter < turnaround + skew) {
            el = color_cast<value_type::format, value_type::rep>(color::color<hsv, uint8_t>(hue_min + val.hue() * (counter - turnaround), val.saturation(), val.value()));
        } else if (counter < turnaround * 2 + skew) {
            el = color_cast<value_type::format, value_type::rep>(color::color<hsv, uint8_t>(hue_max, val.saturation(), val.value()));
        } else {
            el = color_cast<value_type::format, value_type::rep>(color::color<hsv, uint8_t>(hue_max - val.hue() * (counter - (turnaround * 2 + skew)), val.saturation(), val.value()));
        }
    }
}
