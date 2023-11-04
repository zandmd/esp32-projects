#include <stddef.h>
#include <driver/rmt_encoder.h>
#include <driver/rmt_tx.h>
#include <driver/rmt_types.h>
#include <esp_check.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <zandmd/drivers/ws2811.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

#define TAG "ws2811"

using namespace zandmd::drivers;
using namespace zandmd::peripheral_alloc;

static const rmt_transmit_config_t config = {
    .loop_count = 0,
    .flags = {
        .eot_level = false
    }
};

static const rmt_bytes_encoder_config_t bytes_config = {
    .bit0 = {
        {
            .duration0 = 5,
            .level0 = true,
            .duration1 = 20,
            .level1 = false
        }
    },
    .bit1 = {
        {
            .duration0 = 12,
            .level0 = true,
            .duration1 = 13,
            .level1 = false
        }
    },
    .flags = {
        .msb_first = true
    }
};

static const rmt_copy_encoder_config_t copy_config = {
};

static const rmt_symbol_word_t reset_code = {
    {
        .duration0 = 250,
        .level0 = false,
        .duration1 = 250,
        .level1 = false
    }
};

ws2811::ws2811(generic_gpio gpio, bool invert) noexcept : state(RMT_ENCODING_RESET) {
    rmt_encoder_t::encode = &ws2811::encode;
    rmt_encoder_t::reset = &ws2811::reset;
    rmt_encoder_t::del = &ws2811::del;

    rmt_tx_channel_config_t tx_chan_config = {
        .gpio_num = gpio,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,
        .mem_block_symbols = 64,
        .trans_queue_depth = 4,
        .flags = {
            .invert_out = invert,
            .with_dma = false,
            .io_loop_back = false,
            .io_od_mode = false
        },
        .intr_priority = 0
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &chan));
    ESP_ERROR_CHECK(rmt_new_bytes_encoder(&bytes_config, &bytes_encoder));
    ESP_ERROR_CHECK(rmt_new_copy_encoder(&copy_config, &copy_encoder));
    ESP_ERROR_CHECK(rmt_enable(chan));
}

ws2811::~ws2811() noexcept {
    if (chan) {
        ESP_ERROR_CHECK(rmt_disable(chan));
        ESP_ERROR_CHECK(del(this));
        ESP_ERROR_CHECK(rmt_del_channel(chan));
        chan = nullptr;
    }
}

void ws2811::wait() const noexcept {
    ESP_ERROR_CHECK(rmt_tx_wait_all_done(chan, portMAX_DELAY));
}

void ws2811::start(const void *colors, size_t size) noexcept {
    ESP_ERROR_CHECK(rmt_transmit(chan, this, colors, 3 * size, &config));
}

size_t ws2811::encode(rmt_encoder_t *encoder, rmt_channel_handle_t chan, const void *data, size_t size, rmt_encode_state_t *state) noexcept {
    ws2811 *ws = static_cast<ws2811 *>(encoder);
    *state = RMT_ENCODING_RESET;
    size_t encoded = 0;
    rmt_encode_state_t session_state = RMT_ENCODING_RESET;
    if (ws->state == RMT_ENCODING_RESET) {
        encoded = ws->bytes_encoder->encode(ws->bytes_encoder, chan, data, size, &session_state);
        if (session_state & RMT_ENCODING_COMPLETE) {
            ws->state = RMT_ENCODING_COMPLETE;
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {
            *state = static_cast<rmt_encode_state_t>(*state | RMT_ENCODING_MEM_FULL);
        }
    }
    if (!(*state & RMT_ENCODING_MEM_FULL)) {
        encoded += ws->copy_encoder->encode(ws->copy_encoder, chan, &reset_code, sizeof(reset_code), &session_state);
        if (session_state & RMT_ENCODING_COMPLETE) {
            ws->state = RMT_ENCODING_RESET;
            *state = static_cast<rmt_encode_state_t>(*state | RMT_ENCODING_COMPLETE);
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {
            *state = static_cast<rmt_encode_state_t>(*state | RMT_ENCODING_MEM_FULL);
        }
    }
    return encoded;
}

esp_err_t ws2811::reset(rmt_encoder_t *encoder) noexcept {
    ws2811 *ws = static_cast<ws2811 *>(encoder);
    ESP_RETURN_ON_ERROR(rmt_encoder_reset(ws->bytes_encoder), TAG, "rmt_encoder_reset(bytes_encoder) failed");
    ESP_RETURN_ON_ERROR(rmt_encoder_reset(ws->copy_encoder), TAG, "rmt_encoder_reset(copy_encoder) failed");
    ws->state = RMT_ENCODING_RESET;
    return ESP_OK;
}

esp_err_t ws2811::del(rmt_encoder_t *encoder) noexcept {
    ws2811 *ws = static_cast<ws2811 *>(encoder);
    if (ws->copy_encoder) {
        ESP_RETURN_ON_ERROR(rmt_del_encoder(ws->copy_encoder), TAG, "rmt_del_encoder(copy_encoder) failed");
        ws->copy_encoder = nullptr;
    }
    if (ws->bytes_encoder) {
        ESP_RETURN_ON_ERROR(rmt_del_encoder(ws->bytes_encoder), TAG, "rmt_del_encoder(bytes_encoder) failed");
        ws->bytes_encoder = nullptr;
    }
    return ESP_OK;
}
