#ifndef BAROMETER_DRIVER_HPP
#define BAROMETER_DRIVER_HPP

#include <cstdint>

namespace novact {
namespace drivers {

struct BarometerData {
    float pressure;    // hPa
    float temperature; // Celsius
    float altitude;    // meters
};

class BarometerDriver {
public:
    BarometerDriver();
    bool initialize();
    BarometerData readData();

private:
    // Placeholder for hardware specific members (e.g., I2C handles)
};

} // namespace drivers
} // namespace novact

#endif // BAROMETER_DRIVER_HPP