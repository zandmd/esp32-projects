#include <assert.h>
#include <esp_log.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <zandmd/bsp/battery.hpp>
#include <zandmd/bsp/charges.hpp>
#include <zandmd/bsp/leds.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/rocket-launcher/lco_to_pad.hpp>
#include <zandmd/rocket-launcher/pad_main.hpp>
#include <zandmd/rocket-launcher/pad_to_lco.hpp>
#include <zandmd/rocket-launcher/timings.hpp>

#define TAG "pad_main"

using namespace zandmd;
using namespace zandmd::bsp;

void rocket_launcher::pad_main() noexcept {
    peripherals::leds.change_all(leds::booting);
    peripherals::lora.enable();

    // Event handlers
    StaticTimer_t timer_mem;
    TimerHandle_t timer = xTimerCreateStatic("rx timeout", timings::MAX_LATENCY, false, nullptr, [](TimerHandle_t timer) {
        peripherals::lora.cancel();
    }, &timer_mem);
    peripherals::buttons.buttonchange = [](bool state, int num) {
        ESP_LOGE(TAG, "Spurious button %s (%d)", state ? "press" : "release", num);
    };
    peripherals::charges.charge_event = [](charges::mask new_state, charges::mask old_state) {
        for (int i = 0; i < 4; ++i) {
            if (new_state[i] != old_state[i]) {
                ESP_LOGI(TAG, "Charge %d now %s", i, new_state[i] ? "continuous" : "discontinuous");
            }
        }
    };

    // Check battery polarity
    if (peripherals::battery.poll() == battery::backwards) {
        ESP_LOGE(TAG, "Battery polarity is backwards");
        peripherals::leds.change_all(leds::batterywrong);
        pad_to_lco tx;
        tx.battery = pad_to_lco::battery_backwards;
        tx.charges[0] = pad_to_lco::charge_gone;
        tx.charges[1] = pad_to_lco::charge_gone;
        tx.charges[2] = pad_to_lco::charge_gone;
        tx.charges[3] = pad_to_lco::charge_gone;
        while (true) {
            ESP_ERROR_CHECK(esp_task_wdt_reset());
            peripherals::lora << tx;
            vTaskDelay(pdMS_TO_TICKS(timings::TX_PERIOD));
        }
    }

    TickType_t last_tx = xTaskGetTickCount();
    TickType_t last_packet_success = last_tx;
    bool low_battery_logged = false;
    charges::mask fired;
    int tx_since_debug = 0;
    int lost_since_debug = 0;
    while (true) {
        ESP_ERROR_CHECK(esp_task_wdt_reset());
        pad_to_lco tx;

        // Check battery level
        if (peripherals::battery.poll() == battery::good) {
            tx.battery = pad_to_lco::battery_good;
            if (low_battery_logged) {
                low_battery_logged = false;
                ESP_LOGI(TAG, "Battery restored to normal voltages: %d", peripherals::battery.poll_raw());
            }
        } else {
            peripherals::leds.change_all(leds::lowbat);
            tx.battery = pad_to_lco::battery_low;
            if (!low_battery_logged) {
                low_battery_logged = true;
                ESP_LOGE(TAG, "Battery is low: %d", peripherals::battery.poll_raw());
            }
        }

        // Check the charge states
        charges::mask charges = peripherals::charges.check();
        for (int i = 0; i < 4; ++i) {
            if (fired[i]) {
                tx.charges[i] = pad_to_lco::charge_fired;
            } else if (charges[i]) {
                tx.charges[i] = pad_to_lco::charge_continuous;
            } else {
                tx.charges[i] = pad_to_lco::charge_gone;
            }
        }

        // Send the data to LCO
        assert(tx.valid());
        xTaskDelayUntil(&last_tx, timings::TX_PERIOD);
        peripherals::lora << tx;
        ++tx_since_debug;

        // Receive the response from LCO
        lco_to_pad rx;
        assert(xTimerStart(timer, timings::MAX_DELAY) == pdPASS);
        peripherals::lora >> rx;
        assert(xTimerStop(timer, timings::MAX_DELAY) == pdPASS);
        if (rx.valid()) {
            if (tx.battery == pad_to_lco::battery_good) {

                // Update LEDs from LCO
                for (int i = 0; i < 4; ++i) {
                    if (rx.charges[i] == lco_to_pad::charge_armed) {
                        peripherals::leds.change_leds(i, leds::armed);
                    } else if (rx.charges[i] == lco_to_pad::charge_fired) {
                        peripherals::leds.change_leds(i, leds::latched);
                    } else if (tx.charges[i] == pad_to_lco::charge_continuous) {
                        peripherals::leds.change_leds(i, leds::closed);
                    } else {
                        peripherals::leds.change_leds(i, leds::open);
                    }
                }

                // Update arming from LCO
                for (int i = 0; i < 4; ++i) {
                    if (rx.charges[i] == lco_to_pad::charge_armed) {
                        peripherals::charges.arm(charges::mask(1 << i));
                    }
                }

                // Fire from LCO
                if (rx.fire == lco_to_pad::do_fire && !fired.any()) {
                    fired = peripherals::charges.are_armed();
                    ESP_LOGI(TAG, "Fire %s", fired.to_string().c_str());
                    peripherals::charges.fire(fired);
                } else {
                    fired = charges::mask();
                    peripherals::charges.disarm_all();
                }

                // Radio link debugging
                last_packet_success = xTaskGetTickCount();
                if (tx_since_debug > timings::TX_PER_DEBUG) {
                    int rssi;
                    float snr;
                    peripherals::lora.get_debug(rssi, snr);
                    ESP_LOGI(TAG, "Comms stats: packet loss = %.1f %%, round trip time = %ld ms, rssi = %d, SNR = %.2f",
                        static_cast<float>(lost_since_debug) * 100.f / static_cast<float>(tx_since_debug),
                        pdTICKS_TO_MS(last_packet_success - last_tx),
                        rssi,
                        snr);
                    tx_since_debug = 0;
                    lost_since_debug = 0;
                }
            }
        } else {
            ++lost_since_debug;

            // Check time since last packet
            TickType_t now = xTaskGetTickCount();
            if (now - last_packet_success >= timings::COMM_TIMEOUT) {
                peripherals::leds.change_all(leds::commsfail);
                last_packet_success = now; // Throttle log messages
                int rssi;
                float snr;
                peripherals::lora.get_debug(rssi, snr);
                ESP_LOGW(TAG, "Comms timeout (last RSSI = %d, SNR = %.2f)", rssi, snr);
            }

        }
    }
}
