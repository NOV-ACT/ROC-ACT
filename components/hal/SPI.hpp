#pragma once

#include <cstdint>

namespace AP_HAL {

class SPI {
public:
    virtual void    init() = 0;
    virtual void    beginTransaction(uint8_t cs_pin, uint32_t clock_speed, uint8_t mode) = 0;
    virtual void    endTransaction(uint8_t cs_pin) = 0;
    virtual uint8_t transfer(uint8_t data) = 0;
    virtual void    transfer(const uint8_t *tx_buffer, uint8_t *rx_buffer, uint32_t len) = 0;
};

} // namespace AP_HAL