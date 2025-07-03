#include "HAL.hpp"
#include "GPIO.hpp"
#include "SPI.hpp"
#include "I2C.hpp"
#include "UARTDriver.hpp"
#include "Util.hpp"

namespace AP_HAL {

// Define a concrete HAL implementation for our system
class NOVACT_HAL : public HAL {
public:
    NOVACT_HAL();
    void init() override;
};

NOVACT_HAL::NOVACT_HAL() {
    // Initialize pointers to concrete implementations (placeholders for now)
    gpio = nullptr; // Replace with actual GPIO implementation
    spi = nullptr;  // Replace with actual SPI implementation
    i2c = nullptr;  // Replace with actual I2C implementation
    uartA = nullptr; // Replace with actual UART implementation
    util = nullptr; // Replace with actual Util implementation
}

void NOVACT_HAL::init() {
    // Initialize all HAL components
    if (gpio) gpio->init();
    if (spi) spi->init();
    if (i2c) i2c->init();
    if (uartA) uartA->begin(115200); // Example baud rate
    if (util) util->init();
}

// Global HAL instance
static NOVACT_HAL _hal_instance;

HAL& get_hal() {
    return _hal_instance;
}

} // namespace AP_HAL