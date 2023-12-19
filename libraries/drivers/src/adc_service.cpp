#include <limits.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_err.h>
#include <esp_log.h>
#include <hal/adc_types.h>
#include <soc/clk_tree_defs.h>
#include <soc/soc_caps.h>
#include <string.h>
#include <zandmd/drivers/adc_service.hpp>
#include <zandmd/peripheral-alloc/generic_adc.hpp>

#define TAG "adc_service"

using namespace zandmd::drivers;
using namespace zandmd::peripheral_alloc;

adc_service::unit adc_service::units[SOC_ADC_DIGI_CONTROLLER_NUM];

adc_service::adc_service(generic_adc pin, adc_bitwidth_t width, adc_atten_t atten) noexcept : pin(pin), last(INT_MAX) {
    unit &un = units[static_cast<adc_unit_t>(pin)];
    if (un.ref_count++ == 0) {
        const adc_oneshot_unit_init_cfg_t cfg = {
            .unit_id = pin,
            .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
            .ulp_mode = ADC_ULP_MODE_DISABLE
        };
        ESP_ERROR_CHECK(adc_oneshot_new_unit(&cfg, &un.handle));
    }
    const adc_oneshot_chan_cfg_t cfg = {
        .atten = atten,
        .bitwidth = width
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(un.handle, pin, &cfg));
    const adc_cali_line_fitting_config_t cali_cfg = {
        .unit_id = pin,
        .atten = atten,
        .bitwidth = width,
        .default_vref = 0
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_cfg, &cali));
}

adc_service::~adc_service() noexcept {
    ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(cali));
    unit &un = units[static_cast<adc_unit_t>(pin)];
    if (--un.ref_count == 0) {
        ESP_ERROR_CHECK(adc_oneshot_del_unit(un.handle));
    }
}

int adc_service::poll() const noexcept {
    unit &un = units[static_cast<adc_unit_t>(pin)];
    int res;
    esp_err_t err = ESP_ERR_TIMEOUT;
    for (int tries = 3; tries > 0 && err == ESP_ERR_TIMEOUT; --tries) {
        err = adc_oneshot_read(un.handle, pin, &res);
    }
    if (err == ESP_ERR_TIMEOUT && last != INT_MAX) {
        ESP_LOGW(TAG, "Re-using ADC result on pin %d/%d due to timeout", static_cast<adc_unit_t>(pin), static_cast<adc_channel_t>(pin));
        return last;
    }
    last = res;
    ESP_ERROR_CHECK(err);
    return res;
}

float adc_service::poll_volt() const noexcept {
    int mV;
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali, poll(), &mV));
    return static_cast<float>(mV) / 1000.f;
}

float adc_service::poll_volt(float res_high, float res_low) const noexcept {
    return poll_volt() * (res_low + res_high) / res_low;
}

adc_service::unit::unit() noexcept : ref_count(0) {
}
