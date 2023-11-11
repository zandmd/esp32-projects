#include <esp_log.h>
#include <string.h>
#include <zandmd/rocket-launcher/lco_to_pad.hpp>

#define TAG "lco_to_pad"

using namespace zandmd::rocket_launcher;

static const lco_to_pad zero = {
    .charges = { lco_to_pad::charge_zero, lco_to_pad::charge_zero, lco_to_pad::charge_zero, lco_to_pad::charge_zero },
    .fire = lco_to_pad::fire_zero
};

bool lco_to_pad::valid() const noexcept {
    for (int i = 0; i < 4; ++i) {
        if (charges[i] != charge_disarmed && charges[i] != charge_armed && charges[i] != charge_fired) {
            if (memcmp(this, &zero, sizeof(*this))) {
                ESP_LOGW(TAG, "Unknown charge %d state 0x%04X", i, charges[i]);
            }
            return false;
        }
    }
    if (fire != dont_fire && fire != do_fire) {
        ESP_LOGW(TAG, "Unknown fire command 0x%08lX", fire);
        return false;
    }
    return true;
}
