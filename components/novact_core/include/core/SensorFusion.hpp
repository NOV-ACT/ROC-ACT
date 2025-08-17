#ifndef SENSOR_FUSION_HPP
#define SENSOR_FUSION_HPP

#include "drivers/ImuDriver.hpp"
#include "drivers/BarometerDriver.hpp"
#include <chrono> // Required for std::chrono::time_point
#include "fused_sensor_data.pb.h"

namespace novact {
namespace core {

class SensorFusion {
public:
    SensorFusion(drivers::ImuDriver& imu, drivers::BarometerDriver& baro);
    FusedSensorData updateAndPublish(); // Renamed to reflect publishing

private:
    drivers::ImuDriver& imuDriver;
    drivers::BarometerDriver& baroDriver;

    // Placeholder for internal state for fusion algorithms (e.g., Kalman filter variables)
    float lastAltitude;
    std::chrono::time_point<std::chrono::system_clock> lastUpdateTime;
};

} // namespace core
} // namespace novact

#endif // SENSOR_FUSION_HPP