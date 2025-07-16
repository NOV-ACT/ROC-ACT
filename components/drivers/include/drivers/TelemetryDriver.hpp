#ifndef TELEMETRY_DRIVER_HPP
#define TELEMETRY_DRIVER_HPP

#include <vector>
#include <cstdint>

namespace novact {
namespace drivers {

class TelemetryDriver {
public:
    TelemetryDriver();
    bool initialize();
    bool sendData(const uint8_t* data, size_t size);
    // For receiving, we'll provide a way to get raw received bytes.
    // The parsing logic will be handled by the caller using UnitCommProtocol.
    std::vector<uint8_t> receiveData(); // Placeholder for receiving raw bytes

private:
    // Placeholder for hardware specific members (e.g., UART handle, radio module)
    // For demonstration, we'll use a simple buffer to simulate received data.
    std::vector<uint8_t> _receivedBuffer;
};

} // namespace drivers
} // namespace novact

#endif // TELEMETRY_DRIVER_HPP