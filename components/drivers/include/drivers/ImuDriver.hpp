#ifndef IMU_DRIVER_HPP
#define IMU_DRIVER_HPP

#include <cstdint>

namespace novact {
namespace drivers {

struct ImuData {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
};

class ImuDriver {
public:
    ImuDriver();
    bool initialize();
    ImuData readData();

private:
    // Placeholder for hardware specific members (e.g., SPI/I2C handles)
};

} // namespace drivers
} // namespace novact

#endif // IMU_DRIVER_HPP