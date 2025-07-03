#pragma once

#include <cstdint>

namespace AP_HAL {

class GPIO;
class SPI;
class I2C;
class UARTDriver;
class Util;

class HAL {
public:
    HAL() = default;
    virtual ~HAL() = default; // Add virtual destructor
    virtual void init() = 0; // Make init pure virtual

    GPIO* gpio;
    SPI* spi;
    I2C* i2c;
    UARTDriver* uartA; // Example UART
    Util* util;
};

HAL& get_hal();

} // namespace AP_HAL