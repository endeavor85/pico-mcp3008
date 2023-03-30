#include "mcp3008.h"

#include "pico/binary_info.h"

MCP3008::MCP3008(const MCP3008_Config &config) :
        spi_port(config.spi_port),
        spi_cs(config.spi_cs_gpio) {
    // configure SPI interface
    spi_init(spi_port, config.spi_baud);
    gpio_set_function(config.spi_rx_gpio, GPIO_FUNC_SPI);
    gpio_set_function(config.spi_sck_gpio, GPIO_FUNC_SPI);
    gpio_set_function(config.spi_tx_gpio, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_3pins_with_func(config.spi_rx_gpio, config.spi_tx_gpio, config.spi_sck_gpio, GPIO_FUNC_SPI));

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(spi_cs);
    gpio_set_dir(spi_cs, GPIO_OUT);
    gpio_put(spi_cs, 1);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(spi_cs, "SPI CS"));
}

MCP3008::~MCP3008() {
}

// SPI transfer for ADC read
int MCP3008::spiRead(uint8_t channel, bool differential) {
    uint8_t tx_buf[] = {
        0x01,   // see datasheet sec 6.1
        ((differential ? 0 : 1) << 7) | (channel << 4)
        };

    uint8_t rx_buf[3] = {0x00, 0x00, 0x00};

    cs_select(spi_cs);
    spi_write_read_blocking(spi_port, tx_buf, rx_buf, 3);
    cs_deselect(spi_cs);

    return (((uint16_t)(rx_buf[1] & 0x07)) << 8) | rx_buf[2];
}

/*!
 *    @brief  Read single ended ADC channel.
 *    @param  channel
 *    @return -1 if channel < 0 or channel > 7, otherwise ADC (int)
 */
int MCP3008::adc_read_channel(uint8_t channel) {
    if ((channel < 0) || (channel > 7)) {
        return -1;
    }
    return spiRead(channel, false);
}

/*!
 *    @brief  Read differential ADC channel.
 *    @param  diffMode
 *    @return -1 if channel < 0 or channel > 7, otherwise ADC difference (int)
 */
int MCP3008::adc_read_channel_diff(AdcDifferentialMode diffMode) {
    if ((diffMode < 0) || (diffMode > 7)) {
        return -1;
    }
    return spiRead(diffMode, true);
}
