#ifndef MCP3008_H
#define MCP3008_H

#include "hardware/spi.h"
#include "hardware/gpio.h"

struct MCP3008_Config {

  spi_inst_t* spi_port = PICO_DEFAULT_SPI_INSTANCE; // hardware SPI instance to use (spi0 or spi1)

  uint spi_baud = 1000000;                          // SPI baud rate

  uint spi_sck_gpio = PICO_DEFAULT_SPI_SCK_PIN;     // GPIO for SPI Clock
  uint spi_tx_gpio = PICO_DEFAULT_SPI_TX_PIN;       // GPIO for SPI TX (MOSI)
  uint spi_rx_gpio = PICO_DEFAULT_SPI_RX_PIN;       // GPIO for SPI RX (MISO)
  uint spi_cs_gpio = PICO_DEFAULT_SPI_CSN_PIN;      // GPIO for SPI Chip Select
};

enum AdcDifferentialMode {
    CH_0_MINUS_1 = 0,  // channel 0 minus channel 1
    CH_1_MINUS_0 = 1,  // channel 1 minus channel 0
    CH_2_MINUS_3 = 2,  // channel 2 minus channel 3
    CH_3_MINUS_2 = 3,  // channel 3 minus channel 2
    CH_4_MINUS_5 = 4,  // channel 4 minus channel 5
    CH_5_MINUS_4 = 5,  // channel 5 minus channel 4
    CH_6_MINUS_7 = 6,  // channel 6 minus channel 7
    CH_7_MINUS_6 = 7,  // channel 7 minus channel 6
};

class MCP3008 {
public:
    MCP3008(const MCP3008_Config &config);
    ~MCP3008();

    int adcReadChannel(uint8_t channel);
    int adcReadChannelDiff(AdcDifferentialMode diffMode);

    static const uint8_t NUM_CHANNELS = 8;  // number of channels on ADC (MCP3004 = 4, MCP3008 = 8)
    static const uint8_t ADC_RESOLUTION = 10;

private:
    spi_inst_t* spi_port;
    uint8_t spi_cs;

    int spiRead(uint8_t channel, bool differential);

    static inline void cs_select(uint pin_cs) {
        asm volatile("nop \n nop \n nop");
        gpio_put(pin_cs, 0);  // Active low
        asm volatile("nop \n nop \n nop");
    }

    static inline void cs_deselect(uint pin_cs) {
        asm volatile("nop \n nop \n nop");
        gpio_put(pin_cs, 1);
        asm volatile("nop \n nop \n nop");
    }
};

#endif  // MCP3008_H
