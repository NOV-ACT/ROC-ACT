#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "core/Logger.hpp"
#include "drivers/SDCardDriver.hpp"
#include "core/MessagingClient.hpp"
#include <string>
#include <sstream>
#include "sensor_imu.pb.h"
#include "sensor_baro.pb.h"
#include "fused_sensor_data.pb.h"
#include "flight_state.pb.h"
#include "event.pb.h"

extern "C" void logger_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    novact::drivers::SDCardDriver sdCardDriver;
    novact::core::Logger logger(sdCardDriver);
    novact::core::MessagingClient messagingClient;

    if (!sdCardDriver.initialize()) {
        printf("Logger Task: Failed to initialize SD Card Driver!\n");
        vTaskDelete(NULL); // Delete task if initialization fails
    }

    printf("Logger Task: Initializing...\n");

    for (;;) {
        std::stringstream logStream;

        // Read and log IMU data
        std::optional<novact::core::SensorImu> imuData = messagingClient.readImu();
        if (imuData) {
            logStream << "IMU: Accel(" << imuData->accel_x << "," << imuData->accel_y << "," << imuData->accel_z << ") ";
        }

        // Read and log Baro data
        std::optional<novact::core::SensorBaro> baroData = messagingClient.readBaro();
        if (baroData) {
            logStream << "Baro: Pres(" << baroData->pressure << ") Temp(" << baroData->temperature << ") Alt(" << baroData->altitude << ") ";
        }

        // Read and log Fused Sensor data
        std::optional<novact::core::FusedSensorData> fusedData = messagingClient.readFusedSensorData();
        if (fusedData) {
            logStream << "Fused: Roll(" << fusedData->attitude_roll << ") Pitch(" << fusedData->attitude_pitch << ") Yaw(" << fusedData->attitude_yaw << ") Alt(" << fusedData->altitude_fused << ") VSpeed(" << fusedData->vertical_speed << ") ";
        }

        // Read and log Flight State
        std::optional<novact::core::FlightState> flightState = messagingClient.readFlightState();
        if (flightState) {
            logStream << "State: " << static_cast<int>(flightState->state) << " ";
        }

        // Read and log Events
        std::optional<novact::core::Event> event = messagingClient.readEvent();
        if (event) {
            logger.logEvent("Event: " + event->description);
        }

        std::string logEntry = logStream.str();
        if (!logEntry.empty()) {
            logger.log(logEntry);
            printf("Logger Task: Logged data.\n");
        } else {
            printf("Logger Task: No new data to log.\n");
        }

        vTaskDelay(pdMS_TO_TICKS(500)); // Run every 500ms
    }
}