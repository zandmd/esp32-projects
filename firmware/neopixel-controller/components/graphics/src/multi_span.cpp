#include <initializer_list>
#include <zandmd/graphics/multi_span.hpp>

using namespace std;
using namespace zandmd::graphics;

void multi_span::clear() noexcept {
    fill(value_type(0, 0, 0));
}

void multi_span::fill(const value_type &val) noexcept {
    for (auto &el : *this) {
        el = val;
    }
}

bool multi_span::color_wipe(const value_type &val, size_type counter) noexcept {
    if (counter >= size()) {
        return false;
    }
    (*this)[counter] = val;
    return true;
}

void multi_span::stripe(const initializer_list<value_type> &sequence, size_type counter) noexcept {
    for (auto &el : *this) {
        el = sequence.begin()[counter++ % sequence.size()];
    }
}
