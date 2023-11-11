#ifndef ZANDMD_ROCKET_LAUNCHER_LCO_TO_PAD_HPP
#define ZANDMD_ROCKET_LAUNCHER_LCO_TO_PAD_HPP

#include <stdint.h>

namespace zandmd {
    namespace rocket_launcher {
        struct lco_to_pad {
            enum charge_state : uint16_t {
                charge_zero = 0,
                charge_disarmed = 0x4E67,
                charge_armed = 0x9BEC,
                charge_fired = 0xDF95
            };

            enum fire_state : uint32_t {
                fire_zero = 0,
                dont_fire = 0x14E7A4DC,
                do_fire = 0xED689596
            };

            charge_state charges[4];
            fire_state fire;

            bool valid() const noexcept;
        };
    }
}

#endif
