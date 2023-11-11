#include <assert.h>
#include <esp_log.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <zandmd/bsp/charges.hpp>
#include <zandmd/bsp/leds.hpp>
#include <zandmd/bsp/peripherals.hpp>
#include <zandmd/rocket-launcher/lco_main.hpp>
#include <zandmd/rocket-launcher/lco_to_pad.hpp>
#include <zandmd/rocket-launcher/pad_to_lco.hpp>
#include <zandmd/rocket-launcher/timings.hpp>

#define TAG "lco_main"

using namespace zandmd;
using namespace zandmd::bsp;

void rocket_launcher::lco_main() noexcept {
    peripherals::leds.change_all(leds::booting);
    peripherals::lora.enable();

    // Event handlers
    StaticTimer_t timer_mem;
    TimerHandle_t timer = xTimerCreateStatic("rx timeout", portMAX_DELAY, false, nullptr, [](TimerHandle_t timer) {
        peripherals::lora.cancel();
    }, &timer_mem);
    peripherals::buttons.buttonchange = [](bool state, int num) {
        if (num == 5) {
            ESP_LOGE(TAG, "Spurious button %s (5)", state ? "press" : "release");
        } else {
            ESP_LOGI(TAG, "Button %s (%d)", state ? "press" : "release", num);
        }
    };
    peripherals::charges.charge_event = [](charges::mask new_state, charges::mask old_state) {
        ESP_LOGE(TAG, "Spurious charge event: %s -> %s", old_state.to_string().c_str(), new_state.to_string().c_str());
    };

    TickType_t last_packet_success = xTaskGetTickCount();
    bool ready_to_fire = true;
    bool battery_problem_logged = false;
    charges::mask charges_fired;
    bool was_fire_pressed = false;
    while (true) {
        ESP_ERROR_CHECK(esp_task_wdt_reset());

        // Receive the data from PAD
        pad_to_lco rx;
        TickType_t timeout = last_packet_success + timings::COMM_TIMEOUT;
        TickType_t now = xTaskGetTickCount();
        TickType_t delay;
        if (timeout > now + timings::TX_PERIOD) {
            delay = timeout - now;
        } else {
            delay = timings::TX_PERIOD;
        }
        assert(xTimerChangePeriod(timer, delay, timings::MAX_DELAY) == pdPASS);
        assert(xTimerStart(timer, timings::MAX_DELAY) == pdPASS);
        peripherals::lora >> rx;
        assert(xTimerStop(timer, timings::MAX_DELAY) == pdPASS);
        if (rx.valid()) {
            lco_to_pad tx;

            // Check the button states
            charges::mask armed;
            for (int i = 0; i < 4; ++i) {
                if (peripherals::buttons.get_button_state(i) && rx.charges[i] == pad_to_lco::charge_continuous) {
                    if (charges_fired[i]) {
                        tx.charges[i] = lco_to_pad::charge_fired;
                    } else {
                        tx.charges[i] = lco_to_pad::charge_armed;
                        armed[i] = true;
                    }
                } else {
                    tx.charges[i] = lco_to_pad::charge_disarmed;
                    if (charges_fired[i] && !peripherals::buttons.get_button_state(4)) {
                        charges_fired[i] = false;
                    }
                }
            }
            if (peripherals::buttons.get_button_state(4)) {
                if (armed.any() && ready_to_fire && was_fire_pressed) {
                    tx.fire = lco_to_pad::do_fire;
                    ready_to_fire = false;
                    charges_fired |= armed;
                } else {
                    tx.fire = lco_to_pad::dont_fire;
                }
                was_fire_pressed = true;
            } else {
                tx.fire = lco_to_pad::dont_fire;
                if (!ready_to_fire && !was_fire_pressed) {
                    ready_to_fire = true;
                }
                was_fire_pressed = false;
            }

            // Send the response to PAD
            assert(tx.valid());
            peripherals::lora << tx;

            // Check battery level
            switch (rx.battery) {
                case pad_to_lco::battery_low:
                    peripherals::leds.change_all(leds::lowbat);
                    if (!battery_problem_logged) {
                        battery_problem_logged = true;
                        ESP_LOGE(TAG, "Battery is low");
                    }
                    break;
                case pad_to_lco::battery_backwards:
                    peripherals::leds.change_all(leds::batterywrong);
                    if (!battery_problem_logged) {
                        battery_problem_logged = true;
                        ESP_LOGE(TAG, "Battery polarity at PAD is backwards");
                    }
                    break;

                case pad_to_lco::battery_good:
                    // Update LEDs
                    for (int i = 0; i < 4; ++i) {
                        if (tx.charges[i] == lco_to_pad::charge_armed) {
                            peripherals::leds.change_leds(i, leds::armed);
                        } else if (tx.charges[i] == lco_to_pad::charge_fired) {
                            peripherals::leds.change_leds(i, leds::latched);
                        } else if (rx.charges[i] == pad_to_lco::charge_continuous) {
                            peripherals::leds.change_leds(i, leds::closed);
                        } else {
                            peripherals::leds.change_leds(i, leds::open);
                        }
                    }
                    if (battery_problem_logged) {
                        battery_problem_logged = false;
                        ESP_LOGI(TAG, "Battery restored to normal voltages");
                    }
                    break;

                default:
                    break;
            }

            last_packet_success = xTaskGetTickCount();
        } else {
            // Check time since last packet
            now = xTaskGetTickCount();
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
