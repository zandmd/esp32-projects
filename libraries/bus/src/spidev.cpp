#include <algorithm>
#include <stddef.h>
#include <string.h>
#include <driver/spi_master.h>
#include <esp_err.h>
#include <esp_log.h>
#include <zandmd/bus/spidev.hpp>

#define TAG "spidev"
#define TRANSFER_LOG_LEVEL ESP_LOG_VERBOSE

using namespace std;
using namespace zandmd::bus;

spidev::~spidev() noexcept {
    ESP_ERROR_CHECK(spi_bus_remove_device(handle));
}

void spidev::transfer(void *buffer, const void *tx, size_t tx_size, void *rx, size_t rx_size) noexcept {
    transfer_context context = {
        .tx = tx,
        .tx_size = tx_size,
        .rx = rx,
        .rx_size = rx_size
    };
    transfer(buffer, max(tx_size, rx_size), &spidev::transfer_rx, &spidev::transfer_tx, &context);
}

spidev::spidev(spi_device_handle_t handle) noexcept : handle(handle) {
}

void spidev::transfer(void *buffer, size_t size, void (*rx_func)(const void *buffer, void *context), void (*tx_func)(void *buffer, void *context), void *context) noexcept {
    spi_transaction_t trans = {
        .flags = 0,
        .cmd = 0,
        .addr = 0,
        .length = size * 8,
        .rxlength = size * 8,
        .user = nullptr,
        .tx_buffer = buffer,
        .rx_buffer = buffer
    };
    const void *rx;
    void *tx;
    if (size <= 4) {
        trans.flags |= SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA;
        rx = trans.rx_data;
        tx = trans.tx_data;
    } else {
        rx = buffer;
        tx = buffer;
    }
    if (TRANSFER_LOG_LEVEL >= LOG_LOCAL_LEVEL) {
        memset(tx, 0, size);
    }
    tx_func(tx, context);
    if (TRANSFER_LOG_LEVEL >= LOG_LOCAL_LEVEL) {
        for (size_t i = 0; i < size; ++i) {
            ESP_LOG_LEVEL_LOCAL(TRANSFER_LOG_LEVEL, TAG, "tx: 0x%02X", reinterpret_cast<uint8_t *>(tx)[i]);
        }
    }
    ESP_ERROR_CHECK(spi_device_transmit(handle, &trans));
    if (TRANSFER_LOG_LEVEL >= LOG_LOCAL_LEVEL) {
        for (size_t i = 0; i < size; ++i) {
            ESP_LOG_LEVEL_LOCAL(TRANSFER_LOG_LEVEL, TAG, "rx: 0x%02X", reinterpret_cast<const uint8_t *>(rx)[i]);
        }
    }
    rx_func(rx, context);
}

void spidev::transfer_rx(const void *buffer, void *context) {
    transfer_context &xfer = *static_cast<transfer_context *>(context);
    memmove(xfer.rx, buffer, xfer.rx_size);
}

void spidev::transfer_tx(void *buffer, void *context) {
    transfer_context &xfer = *static_cast<transfer_context *>(context);
    memmove(buffer, xfer.tx, xfer.tx_size);
}
