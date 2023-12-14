#include <esp_err.h>
#include <nvs_flash.h>
#include <zandmd/drivers/nonvol.hpp>

using namespace zandmd::drivers;

nonvol::nonvol() noexcept {
    esp_err_t res = nvs_flash_init();
    if (res == ESP_ERR_NVS_NO_FREE_PAGES || res == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        res = nvs_flash_init();
    }
    ESP_ERROR_CHECK(res);
}

nonvol::~nonvol() noexcept {
    ESP_ERROR_CHECK(nvs_flash_deinit());
}
