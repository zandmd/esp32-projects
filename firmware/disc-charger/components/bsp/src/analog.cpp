#include <hal/adc_types.h>
#include <zandmd/bsp/analog.hpp>
#include <zandmd/bsp/gpio.hpp>

using namespace zandmd::bsp;

const float analog::battery_res_high;
const float analog::battery_res_low;
const float analog::usb_res_high;
const float analog::usb_res_low;
const float analog::usb_threshold_volt;
const float analog::battery_charged_volt;
const float analog::battery_critical_volt;
const float analog::battery_low_volt;

analog::analog() noexcept : adc_5v(gpio::adc_5v, ADC_BITWIDTH_10), adc_vbat(gpio::adc_bat, ADC_BITWIDTH_10) {
}

float analog::battery_voltage() const noexcept {
    return adc_vbat.poll_volt(battery_res_high, battery_res_low);
}

float analog::usb_voltage() const noexcept {
    return adc_5v.poll_volt(usb_res_high, usb_res_low);
}

analog::battery_state analog::battery() const noexcept {
    float volt = battery_voltage();
    if (volt >= battery_charged_volt) {
        return charged;
    } else if (volt >= battery_low_volt) {
        return good;
    } else if (volt >= battery_critical_volt) {
        return low;
    } else {
        return critical;
    }
}

bool analog::usb() const noexcept {
    return usb_voltage() >= usb_threshold_volt;
}
