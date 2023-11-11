#include <esp_log.h>
#include <string.h>
#include <zandmd/rocket-launcher/pad_to_lco.hpp>

#define TAG "pad_to_lco"

using namespace zandmd::rocket_launcher;

static const pad_to_lco zero = {
    .battery = pad_to_lco::battery_zero,
    .comms = pad_to_lco::comm_zero,
    .charges = { pad_to_lco::charge_zero, pad_to_lco::charge_zero, pad_to_lco::charge_zero, pad_to_lco::charge_zero }
};

bool pad_to_lco::valid() const noexcept {
    if (battery != battery_good && battery != battery_low && battery != battery_backwards) {
        if (memcmp(this, &zero, sizeof(*this))) {
            ESP_LOGW(TAG, "Unknown battery value 0x%04X", battery);
        }
        return false;
    }
    if (comms != comm_good && comms != comm_bad) {
        ESP_LOGW(TAG, "Unknown comms value 0x%04X", comms);
        return false;
    }
    for (int i = 0; i < 4; ++i) {
        if (charges[i] != charge_gone && charges[i] != charge_continuous && charges[i] != charge_fired) {
            ESP_LOGW(TAG, "Unknown charge %d state 0x%04X", i, charges[i]);
            return false;
        }
    }
    return true;
}
