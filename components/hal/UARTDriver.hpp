#pragma once

#include <cstdint>
#include <cstddef> // For size_t

namespace AP_HAL {

class UARTDriver {
public:
    virtual void    begin(uint32_t baud) = 0;
    virtual void    end() = 0;
    virtual size_t  write(const uint8_t *buffer, size_t size) = 0;
    virtual int     read() = 0; // Reads a single byte, returns -1 if no data
    virtual bool    available() = 0;
    virtual void    flush() = 0;
};

} // namespace AP_HAL