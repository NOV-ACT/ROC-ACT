#include "StateManager.hpp"
#include "sensor_imu.pb.h"
#include "sensor_baro.pb.h"
#include "pyro_command.pb.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cmath> // For fabs

// FSM Constants
constexpr float LAUNCH_ACCELERATION_THRESHOLD_G = 3.0f;
constexpr float MOTOR_BURNOUT_ACCELERATION_THRESHOLD_G = 1.5f;
constexpr int APOGEE_DESCENT_READINGS_THRESHOLD = 5;
constexpr uint32_t DROGUE_DEPLOY_TIMEOUT_MS = 5000; // 5 seconds after coast starts
constexpr float MAIN_DEPLOY_ALTITUDE_M = 300.0f;

namespace novact {
namespace core {

StateManager::StateManager() :
    currentState(FlightPhase::FlightPhase_IDLE),
    launch_time_ms(0),
    coast_start_time_ms(0),
    max_altitude(0.0f),
    last_altitude(0.0f),
    consecutive_descent_readings(0),
    main_pyro_fired(false),
    drogue_pyro_fired(false)
{
    // Constructor
}

FlightPhase StateManager::updateState(MessagingClient& client) {
    SensorImu imuData;
    SensorBaro baroData;

    // Always try to read the latest sensor data
    client.readImu(client.subscribeImu().value(), imuData);
    client.readBaro(client.subscribeBaro().value(), baroData);

    float current_altitude = baroData.altitude_m;
    float total_acceleration = std::sqrt(std::pow(imuData.accel_x, 2) + std::pow(imuData.accel_y, 2) + std::pow(imuData.accel_z, 2)) / 9.81f;

    switch (currentState){
        case FlightPhase_IDLE:
            if (total_acceleration > LAUNCH_ACCELERATION_THRESHOLD_G) {
                currentState = FlightPhase_BOOST;
                launch_time_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
            }
            break;  
        case FlightPhase_BOOST:
            if (total_acceleration < MOTOR_BURNOUT_ACCELERATION_THRESHOLD_G) {
                currentState = FlightPhase_COAST;
                coast_start_time_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
            }
            break;
        case FlightPhase_COAST:
            // Apogee detection logic
            if (current_altitude > max_altitude) {
                max_altitude = current_altitude;
                consecutive_descent_readings = 0;
            } else {
                consecutive_descent_readings++;             
            }
            if (consecutive_descent_readings >= APOGEE_DESCENT_READINGS_THRESHOLD) {
                currentState = FlightPhase_DROGUE_DEPLOY;
            }
            // Timeout failsafe
            if (xTaskGetTickCount() * portTICK_PERIOD_MS - coast_start_time_ms > DROGUE_DEPLOY_TIMEOUT_MS) {
                currentState = FlightPhase_DROGUE_DEPLOY;
            }
            break;
        case FlightPhase_DROGUE_DEPLOY:
            if (current_altitude <= MAIN_DEPLOY_ALTITUDE_M && drogue_pyro_fired) {
                currentState = FlightPhase_MAIN_DEPLOY;
            }
            break;
        case FlightPhase_MAIN_DEPLOY:
            if (fabs(current_altitude - last_altitude) < 1.0f && main_pyro_fired) { // Landed
                currentState = FlightPhase_RECOVERY;
            }
            break;
        case FlightPhase_RECOVERY:
            // Recovery phase logic, if any
            break;
        default:
            // Handle unexpected states
            currentState = FlightPhase_IDLE;
            break;
    }
    last_altitude = current_altitude;
    return currentState;
}

void StateManager::checkAndFirePyros(MessagingClient& client) {
    if (currentState == FlightPhase::FlightPhase_DROGUE_DEPLOY && !drogue_pyro_fired) {
        PyroCommand command = PyroCommand_init_zero;
        command.channel_id = 1; // Drogue channel
        command.activate = true; // Fire the drogue pyro
        command.timestamp_us = xTaskGetTickCount() * portTICK_PERIOD_MS * 1000; // Convert to microseconds
        client.publishPyroCommand(command);
        drogue_pyro_fired = true;
    }

    if (currentState == FlightPhase::FlightPhase_MAIN_DEPLOY && !main_pyro_fired) {
        PyroCommand command = PyroCommand_init_zero;
        command.channel_id = 1; // Drogue channel
        command.activate = true; // Fire the drogue pyro
        command.timestamp_us = xTaskGetTickCount() * portTICK_PERIOD_MS * 1000; // Convert to microseconds
        client.publishPyroCommand(command);
        main_pyro_fired = true;
    }
}

FlightPhase StateManager::getCurrentState() const {
    return currentState;
}

} // namespace core
} // namespace novact