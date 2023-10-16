#ifndef ZANDMD_BUS_DUPLEX_HPP
#define ZANDMD_BUS_DUPLEX_HPP

namespace zandmd {
    namespace bus {
        struct full_duplex_type {
        };

        static const full_duplex_type full_duplex;

        struct half_duplex_type {
        };

        static const half_duplex_type half_duplex;
    }
}

#endif
