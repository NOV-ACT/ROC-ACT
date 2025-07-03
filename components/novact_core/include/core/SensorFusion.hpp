#ifndef SENSOR_FUSION_HPP
#define SENSOR_FUSION_HPP

#include "drivers/ImuDriver.hpp"
#include "drivers/BarometerDriver.hpp"
#include "core/MessagingClient.hpp" // For publishing fused data
#include <chrono> // Required for std::chrono::time_point

namespace novact {
namespace core {

#include "fused_sensor_data.pb.h"

class SensorFusion {
public:
    SensorFusion(drivers::ImuDriver& imu, drivers::BarometerDriver& baro, MessagingClient& msgClient);
    FusedSensorData updateAndPublish(); // Renamed to reflect publishing

private:
    drivers::ImuDriver& imuDriver;
    drivers::BarometerDriver& baroDriver;
    MessagingClient& messagingClient; // Reference to messaging client

    // Placeholder for internal state for fusion algorithms (e.g., Kalman filter variables)
    float lastAltitude;
    std::chrono::time_point<std::chrono::system_clock> lastUpdateTime;
};

} // namespace core
} // namespace novact

#endif // SENSOR_FUSION_HPP