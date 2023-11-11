#ifndef ZANDMD_ROCKET_LAUNCHER_PAD_TO_LCO_HPP
#define ZANDMD_ROCKET_LAUNCHER_PAD_TO_LCO_HPP

#include <stdint.h>

namespace zandmd {
    namespace rocket_launcher {
        struct pad_to_lco {
            enum battery_state : uint16_t {
                battery_zero = 0,
                battery_good = 0x01A9,
                battery_low = 0x3E4B,
                battery_backwards = 0xF2F2
            };

            enum charge_state : uint16_t {
                charge_zero = 0,
                charge_gone = 0x20DC,
                charge_continuous = 0xDF50,
                charge_fired = 0xE206
            };

            battery_state battery;
            charge_state charges[4];

            bool valid() const noexcept;
        };
    }
}

#endif
