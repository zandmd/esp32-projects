#include <driver/spi_common.h>
#include <driver/spi_master.h>
#include <esp_err.h>
#include <hal/spi_types.h>
#include <zandmd/bus/spi.hpp>
#include <zandmd/bus/spidev.hpp>
#include <zandmd/peripheral-alloc/generic_gpio.hpp>

using namespace zandmd::bus;
using namespace zandmd::peripheral_alloc;

spi::builder::operator spidev() noexcept {
    spi_device_handle_t handle;
    ESP_ERROR_CHECK(spi_bus_add_device(dev, &config, &handle));
    return spidev(handle);
}

spi::builder::builder(spi_host_device_t dev, generic_gpio cs) noexcept : dev(dev) {
    config = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = 0,
        .clock_source = SPI_CLK_SRC_DEFAULT,
        .duty_cycle_pos = 0,
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 0,
        .clock_speed_hz = SPI_MASTER_FREQ_8M,
        .input_delay_ns = 0,
        .spics_io_num = cs,
        .flags = 0,
        .queue_size = 1,
        .pre_cb = nullptr,
        .post_cb = nullptr
    };
}

spi::~spi() noexcept {
    ESP_ERROR_CHECK(spi_bus_free(dev));
}

spi::builder spi::operator [](generic_gpio cs) noexcept {
    return builder(dev, cs);
}

spi::spi(spi_host_device_t dev, generic_gpio mosi, generic_gpio miso, generic_gpio sclk) noexcept
        : dev(dev) {
    spi_bus_config_t config = {
        .mosi_io_num = mosi,
        .miso_io_num = miso,
        .sclk_io_num = sclk,
        .data2_io_num = -1,
        .data3_io_num = -1,
        .data4_io_num = -1,
        .data5_io_num = -1,
        .data6_io_num = -1,
        .data7_io_num = -1,
        .max_transfer_sz = 0,
        .flags = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_IOMUX_PINS | SPICOMMON_BUSFLAG_SCLK | SPICOMMON_BUSFLAG_MISO | SPICOMMON_BUSFLAG_MOSI,
        .isr_cpu_id = ESP_INTR_CPU_AFFINITY_AUTO,
        .intr_flags = 0
    };
    ESP_ERROR_CHECK(spi_bus_initialize(dev, &config, SPI_DMA_CH_AUTO));
}
