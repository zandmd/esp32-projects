#include <stdint.h>
#include <zandmd/math/random.hpp>

using namespace zandmd::math;

random::random(uint32_t state) noexcept : state(state) {
}

random::operator uint32_t() noexcept {
    // https://en.wikipedia.org/wiki/Xorshift#Example_implementation
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}
