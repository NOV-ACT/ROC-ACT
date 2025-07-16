#include "SensorFusion.hpp"
#include <iostream> // For logging/debugging, replace with actual logging/event system
#include <numeric>  // For std::accumulate or similar if needed for fusion
#include <chrono>   // For std::chrono::system_clock
#include "freertos/FreeRTOS.h"// For xTaskGetTickCount
#include "freertos/task.h"
#include "drivers/ImuDriver.hpp"
#include "drivers/BarometerDriver.hpp"
#include "MessagingClient.hpp"

namespace novact {
namespace core {

SensorFusion::SensorFusion(drivers::ImuDriver& imu, drivers::BarometerDriver& baro, MessagingClient& msgClient)
    : imuDriver(imu), baroDriver(baro), messagingClient(msgClient), lastAltitude(0.0f), lastUpdateTime(std::chrono::system_clock::now()) {
    std::cout << "SensorFusion initialized." << std::endl;
}

FusedSensorData SensorFusion::updateAndPublish() {
    // Read raw data from drivers
    // drivers::ImuData imuData = imuDriver.readData(); // Commented out as it's currently unused
    drivers::BarometerData baroData = baroDriver.readData();

    // Placeholder for actual sensor fusion algorithms (e.g., Kalman filter, complementary filter)
    // For demonstration, a simple combination and altitude calculation
    //FusedSensorData fusedData;

    // Attitude estimation (simplified: direct IMU gyro/accel for demonstration)
    // In a real system, this would involve complex algorithms like Madgwick or Mahony filters.
    //fusedData.attitude_roll = imuData.gyro_x * 0.01f;  // Dummy calculation
    //fusedData.attitude_pitch = imuData.gyro_y * 0.01f; // Dummy calculation
    //fusedData.attitude_yaw = imuData.gyro_z * 0.01f;   // Dummy calculation

    // Altitude and vertical speed estimation
    // Simple altitude from barometer, vertical speed from change over time
    float currentAltitude = baroData.altitude;
    auto currentTime = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedSeconds = currentTime - lastUpdateTime;

    if (elapsedSeconds.count() > 0) {
        //fusedData.vertical_speed = (currentAltitude - lastAltitude) / elapsedSeconds.count();
    } else {
        //fusedData.vertical_speed = 0.0f;
    }

    //fusedData.altitude_fused = currentAltitude; // For now, direct barometer altitude
    //fusedData.timestamp = xTaskGetTickCount(); // Use FreeRTOS tick count as timestamp

    lastAltitude = currentAltitude;
    lastUpdateTime = currentTime;

    // Publish fused data
    //messagingClient.publishFusedSensorData(fusedData);

    std::cout << "SensorFusion: Updated and published data." << std::endl;
    return {} ; //fusedData;
}

} // namespace core
} // namespace novact