#ifndef ZANDMD_ROCKET_LAUNCHER_PAD_TO_LCO_HPP
#define ZANDMD_ROCKET_LAUNCHER_PAD_TO_LCO_HPP

#include <stdint.h>

namespace zandmd {
    namespace rocket_launcher {
        struct pad_to_lco {
            enum battery_state : uint16_t {
                battery_good = 0x01A9,
                battery_low = 0x3E4B,
                battery_backwards = 0xF2F2
            };

            enum charge_state : uint16_t {
                charge_gone = 0x20DC,
                charge_continuous = 0xDF50
            };

            battery_state battery;
            charge_state charges[4];

            inline bool valid() const noexcept {
                if (battery != battery_good && battery != battery_low && battery != battery_backwards) {
                    return false;
                }
                for (int i = 0; i < 4; ++i) {
                    if (charges[i] != charge_gone && charges[i] != charge_continuous) {
                        return false;
                    }
                }
                return true;
            }
        };
    }
}

#endif
