#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/rocket-launcher/lco_main.hpp>
#include <zandmd/rocket-launcher/pad_main.hpp>

#define TAG "main"

using namespace zandmd::bsp;
using namespace zandmd::rocket_launcher;

extern "C" void app_main() {
    // Wait for other tasks to init
    vTaskDelay(pdMS_TO_TICKS(200));

    if (peripherals::buttons.get_button_state(5)) {
        ESP_LOGI(TAG, "Booting up in PAD mode");
        pad_main();
    } else {
        ESP_LOGI(TAG, "Booting up in LCO mode");
        lco_main();
    }
}
