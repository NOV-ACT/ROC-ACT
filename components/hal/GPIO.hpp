#pragma once

#include <cstdint>

namespace AP_HAL {

class GPIO {
public:
    enum GPIO_PinMode {
        INPUT,
        OUTPUT,
        INPUT_PULLUP,
        INPUT_PULLDOWN
    };

    virtual void    init() = 0;
    virtual void    pinMode(uint8_t pin, GPIO_PinMode mode) = 0;
    virtual void    write(uint8_t pin, uint8_t value) = 0;
    virtual uint8_t read(uint8_t pin) = 0;
    virtual void    toggle(uint8_t pin) = 0;
};

} // namespace AP_HAL