#include <esp_adc/adc_oneshot.h>
#include <esp_err.h>
#include <hal/adc_types.h>
#include <soc/clk_tree_defs.h>
#include <soc/soc_caps.h>
#include <zandmd/drivers/adc_service.hpp>
#include <zandmd/peripheral-alloc/generic_adc.hpp>

using namespace zandmd::drivers;
using namespace zandmd::peripheral_alloc;

adc_service::unit adc_service::units[SOC_ADC_DIGI_CONTROLLER_NUM];

adc_service::adc_service(generic_adc pin, adc_bitwidth_t width, adc_atten_t atten) noexcept : pin(pin) {
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
}

adc_service::~adc_service() noexcept {
    unit &un = units[static_cast<adc_unit_t>(pin)];
    if (--un.ref_count == 0) {
        ESP_ERROR_CHECK(adc_oneshot_del_unit(un.handle));
    }
}

int adc_service::poll() const noexcept {
    unit &un = units[static_cast<adc_unit_t>(pin)];
    int res;
    ESP_ERROR_CHECK(adc_oneshot_read(un.handle, pin, &res));
    return res;
}

adc_service::unit::unit() noexcept : ref_count(0) {
}
