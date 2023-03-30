#include <iostream>
#include <iomanip>

#include "hardware/spi.h"

#include "pico/stdlib.h"
#include "pico/binary_info.h"

#include "mcp3008.h"

using std::cout;
using std::endl;

int main() {
    stdio_init_all();

    sleep_ms(1000);

    cout << endl
         << "***********************" << endl
         << "*** MCP3008 Example ***" << endl
         << "***********************" << endl
         << endl;


    cout << "Reading ADC values via SPI..." << endl;
    
    MCP3008_Config mcp3008_config;
    mcp3008_config.spi_port = PICO_DEFAULT_SPI_INSTANCE;
    
    cout << "PIN_MISO on GPIO" << mcp3008_config.spi_rx_gpio << endl;
    cout << "PIN_CS   on GPIO" << mcp3008_config.spi_cs_gpio   << endl;
    cout << "PIN_SCK  on GPIO" << mcp3008_config.spi_sck_gpio  << endl;
    cout << "PIN_MOSI on GPIO" << mcp3008_config.spi_tx_gpio << endl;

    MCP3008 mcp3008(mcp3008_config);

    cout << "ch0, ch1, ch2, ch3, ch4, ch5, ch6, ch7" << endl;

    while (true) {

        for (int channel = 0; channel < MCP3008::NUM_CHANNELS; channel++) {
            cout << std::fixed << std::setw(4)
                << mcp3008.adc_read_channel(channel);
            if(channel < MCP3008::NUM_CHANNELS - 1) {
                cout << ", ";
            }
        }
        cout << endl;

        sleep_ms(500);
    }

    return 0;
}
