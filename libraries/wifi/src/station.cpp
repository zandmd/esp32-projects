#include <esp_err.h>
#include <esp_event.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <stdint.h>
#include <string.h>
#include <zandmd/drivers/event_loop.hpp>
#include <zandmd/drivers/nonvol.hpp>
#include <zandmd/secrets/wifi.hpp>
#include <zandmd/wifi/station.hpp>

using namespace zandmd;
using namespace zandmd::drivers;
using namespace zandmd::wifi;

station::station(event_loop &evl, nonvol &nonvol) noexcept {
}

station::~station() noexcept {
    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_ERROR_CHECK(esp_netif_deinit());
}

void station::event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_CONNECTED:
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                esp_wifi_connect();
                break;
            default:
                break;
        }
    } else if (event_base == IP_EVENT) {
        switch (event_id) {
            case IP_EVENT_STA_GOT_IP:
                break;
            default:
                break;
        }
    }
}

void station::post_rtos_init() noexcept {
    ESP_ERROR_CHECK(esp_netif_init());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &station::event_handler, this, &wifi_event));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &station::event_handler, this, &ip_event));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = "",
            .scan_method = WIFI_FAST_SCAN,
            .bssid_set = false,
            .bssid = "",
            .channel = 0,
            .listen_interval = 3,
            .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
            .threshold = {
                .rssi = -127,
                .authmode = WIFI_AUTH_WPA2_PSK
            },
            .pmf_cfg = {
                .capable = false,
                .required = false
            },
            .rm_enabled = false,
            .btm_enabled = false,
            .mbo_enabled = false,
            .ft_enabled = false,
            .owe_enabled = false,
            .transition_disable = false,
            .reserved = 0,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
            .sae_pk_mode = WPA3_SAE_PK_MODE_AUTOMATIC,
            .failure_retry_cnt = 0,
            .he_dcm_set = false,
            .he_dcm_max_constellation_tx = 0,
            .he_dcm_max_constellation_rx = 0,
            .he_mcs9_enabled = false,
            .he_su_beamformee_disabled = false,
            .he_trig_su_bmforming_feedback_disabled = false,
            .he_trig_mu_bmforming_partial_feedback_disabled = false,
            .he_trig_cqi_feedback_disabled = false,
            .he_reserved = 0,
            .sae_h2e_identifier = ""
        }
    };
    strcpy(reinterpret_cast<char *>(wifi_config.sta.ssid), secrets::wifi::ssid);
    strcpy(reinterpret_cast<char *>(wifi_config.sta.password), secrets::wifi::password);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}
