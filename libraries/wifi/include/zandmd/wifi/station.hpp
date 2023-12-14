#ifndef ZANDMD_WIFI_STATION_HPP
#define ZANDMD_WIFI_STATION_HPP

#include <esp_event.h>
#include <stdint.h>
#include <zandmd/drivers/event_loop.hpp>
#include <zandmd/drivers/nonvol.hpp>
#include <zandmd/drivers/rtos_init.hpp>
#include <zandmd/wifi/network.hpp>

namespace zandmd {
    namespace wifi {
        class station : public network, private drivers::rtos_init {
            public:
                station(drivers::event_loop &evl, drivers::nonvol &nonvol) noexcept;
                ~station() noexcept;

            private:
                static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) noexcept;
                void post_rtos_init() noexcept override;

                esp_event_handler_instance_t wifi_event;
                esp_event_handler_instance_t ip_event;
        };
    }
}

#endif
