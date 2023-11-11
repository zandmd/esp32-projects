#include <assert.h>
#include <esp_log.h>
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
            peripherals::lora << tx;
            vTaskDelay(pdMS_TO_TICKS(timings::TX_PERIOD));
        }
    }

    TickType_t last_packet_success = xTaskGetTickCount();
    bool low_battery_logged = false;
    while (true) {
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
                ESP_LOGE(TAG, "Battery is low: %d", peripherals::battery.poll_raw());
            }
        }

        // Check the charge states
        charges::mask charges = peripherals::charges.check();
        for (int i = 0; i < 4; ++i) {
            if (charges[i]) {
                tx.charges[i] = pad_to_lco::charge_continuous;
            } else {
                tx.charges[i] = pad_to_lco::charge_gone;
            }
        }

        // Send the data to LCO
        assert(tx.valid());
        peripherals::lora << tx;

        // Receive the response from LCO
        lco_to_pad rx;
        assert(xTimerStart(timer, timings::MAX_DELAY) == pdPASS);
        peripherals::lora >> rx;
        assert(xTimerStop(timer, timings::MAX_DELAY) == pdPASS);
        if (rx.valid() && tx.battery == pad_to_lco::battery_good) {

            // Update LEDs from LCO
            for (int i = 0; i < 4; ++i) {
                if (rx.charges[i] == lco_to_pad::charge_armed) {
                    peripherals::leds.change_leds(i, leds::armed);
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
            if (rx.fire == lco_to_pad::do_fire) {
                ESP_LOGI(TAG, "Fire %s", peripherals::charges.are_armed().to_string().c_str());
                peripherals::charges.fire(peripherals::charges.are_armed());
            } else {
                peripherals::charges.disarm_all();
            }

            last_packet_success = xTaskGetTickCount();
        } else {
            // Check time since last packet
            TickType_t now = xTaskGetTickCount();
            if (now - last_packet_success >= timings::COMM_TIMEOUT) {
                peripherals::leds.change_all(leds::commsfail);
                last_packet_success = now; // Throttle log messages
                int rssi;
                float snr;
                peripherals::lora.get_debug(rssi, snr);
                ESP_LOGW(TAG, "Comms timeout (last RSSI = %d, SNR = %f)", rssi, snr);
            }

        }
    }
}