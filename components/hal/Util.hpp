#pragma once

#include <cstdint>

namespace AP_HAL {

class Util {
public:
    virtual void    init() = 0;
    virtual uint64_t micros() = 0;
    virtual uint32_t millis() = 0;
    virtual void    delay_micros(uint32_t us) = 0;
    virtual void    delay_millis(uint32_t ms) = 0;
};

} // namespace AP_HAL