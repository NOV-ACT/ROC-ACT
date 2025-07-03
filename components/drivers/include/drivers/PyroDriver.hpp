#ifndef PYRO_DRIVER_HPP
#define PYRO_DRIVER_HPP

#include <cstdint>

namespace novact {
namespace drivers {

class PyroDriver {
public:
    PyroDriver();
    bool initialize();
    void activateChannel(uint8_t channel);
    void deactivateChannel(uint8_t channel);

private:
    // Placeholder for hardware specific members (e.g., GPIO pins)
};

} // namespace drivers
} // namespace novact

#endif // PYRO_DRIVER_HPP