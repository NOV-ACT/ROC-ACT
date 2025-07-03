#ifndef TELEMETRY_DRIVER_HPP
#define TELEMETRY_DRIVER_HPP

#include <string>
#include <cstdint>

namespace novact {
namespace drivers {

class TelemetryDriver {
public:
    TelemetryDriver();
    bool initialize();
    bool sendData(const std::string& data);

private:
    // Placeholder for hardware specific members (e.g., UART handle, radio module)
};

} // namespace drivers
} // namespace novact

#endif // TELEMETRY_DRIVER_HPP