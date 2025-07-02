#include "PyroDriver.hpp"
#include <iostream> // For demonstration, replace with actual hardware access

namespace novact {
namespace drivers {

PyroDriver::PyroDriver() {
    std::cout << "PyroDriver initialized." << std::endl;
}

bool PyroDriver::initialize() {
    std::cout << "PyroDriver: Initializing hardware (GPIOs)..." << std::endl;
    return true;
}

void PyroDriver::activateChannel(uint8_t channel) {
    std::cout << "PyroDriver: Activating channel " << static_cast<int>(channel) << std::endl;
    // Placeholder for actual GPIO activation
}

void PyroDriver::deactivateChannel(uint8_t channel) {
    std::cout << "PyroDriver: Deactivating channel " << static_cast<int>(channel) << std::endl;
    // Placeholder for actual GPIO deactivation
}

} // namespace drivers
} // namespace novact