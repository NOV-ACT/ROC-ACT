#pragma once

#include <cstdint>

namespace AP_HAL {

class I2C {
public:
    enum I2C_BusSpeed {
        SPEED_100KHZ,
        SPEED_400KHZ,
        SPEED_1MHZ
    };

    virtual void    init() = 0;
    virtual void    beginTransaction(uint8_t addr, I2C_BusSpeed speed) = 0;
    virtual void    endTransaction() = 0;
    virtual uint8_t read(uint8_t addr, uint8_t reg, uint8_t *data, uint32_t len) = 0;
    virtual uint8_t write(uint8_t addr, uint8_t reg, const uint8_t *data, uint32_t len) = 0;
    virtual uint8_t writeRegister(uint8_t addr, uint8_t reg, uint8_t value) = 0;
    virtual uint8_t readRegister(uint8_t addr, uint8_t reg) = 0;
};

} // namespace AP_HAL