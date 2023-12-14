#ifndef ZANDMD_SECRETS_WIFI_HPP
#define ZANDMD_SECRETS_WIFI_HPP

namespace zandmd {
    namespace secrets {
        class wifi {
            public:
                static const char *const ssid;
                static const char *const password;

            private:
                wifi() noexcept;
        };
    }
}

#endif
