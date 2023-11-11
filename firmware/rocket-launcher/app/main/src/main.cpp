#include <esp_err.h>
#include <esp_log.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <soc/soc_caps.h>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/bsp/tasks.hpp>
#include <zandmd/rocket-launcher/lco_main.hpp>
#include <zandmd/rocket-launcher/pad_main.hpp>
#include <zandmd/rocket-launcher/timings.hpp>

#define TAG "main"

using namespace zandmd::bsp;
using namespace zandmd::rocket_launcher;

extern "C" void app_main() {
    // Wait for other tasks to init
    vTaskDelay(pdMS_TO_TICKS(200));

    // Start up the watchdog
    const esp_task_wdt_config_t cfg = {
        .timeout_ms = pdTICKS_TO_MS(timings::COMM_TIMEOUT + timings::TX_PERIOD + 1),
        .idle_core_mask = (1 << SOC_CPU_CORES_NUM) - 1,
        .trigger_panic = true
    };
    ESP_ERROR_CHECK(esp_task_wdt_reconfigure(&cfg));
    ESP_ERROR_CHECK(esp_task_wdt_add(nullptr));
    peripherals::buttons.enable_watchdog();
    peripherals::charges.enable_watchdog();
    peripherals::leds.enable_watchdog();

    // Launch main task
    vTaskPrioritySet(nullptr, tasks::maintask);
    if (peripherals::buttons.get_button_state(5)) {
        ESP_LOGI(TAG, "Booting up in PAD mode");
        pad_main();
    } else {
        ESP_LOGI(TAG, "Booting up in LCO mode");
        lco_main();
    }
}
