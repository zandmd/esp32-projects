#ifndef ZANDMD_WIFI_NETWORK_HPP
#define ZANDMD_WIFI_NETWORK_HPP

namespace zandmd {
    namespace wifi {
        class network {
            friend class station;

            private:
                constexpr network() noexcept = default;
        };
    }
}

#endif
