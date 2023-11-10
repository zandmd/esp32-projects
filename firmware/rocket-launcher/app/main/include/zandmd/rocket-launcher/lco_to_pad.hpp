#ifndef ZANDMD_ROCKET_LAUNCHER_LCO_TO_PAD_HPP
#define ZANDMD_ROCKET_LAUNCHER_LCO_TO_PAD_HPP

#include <stdint.h>

namespace zandmd {
    namespace rocket_launcher {
        struct lco_to_pad {
            enum charge_state : uint16_t {
                charge_disarmed = 0x4E67,
                charge_armed = 0x9BEC
            };

            enum fire_state : uint32_t {
                dont_fire = 0x14E7A4DC,
                do_fire = 0xED689596
            };

            charge_state charges[4];
            fire_state fire;

            inline bool valid() const noexcept {
                for (int i = 0; i < 4; ++i) {
                    if (charges[i] != charge_disarmed && charges[i] != charge_armed) {
                        return false;
                    }
                }
                if (fire != dont_fire && fire != do_fire) {
                    return false;
                }
                return true;
            }
        };
    }
}

#endif
