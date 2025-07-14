#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
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

    // Subscribe to all topics that LoggerTask needs to read
    std::optional<size_t> imuSubToken = messagingClient.subscribeImu();
    std::optional<size_t> baroSubToken = messagingClient.subscribeBaro();
    std::optional<size_t> fusedSensorDataSubToken = messagingClient.subscribeFusedSensorData();
    std::optional<size_t> flightStateSubToken = messagingClient.subscribeFlightState();
    std::optional<size_t> eventSubToken = messagingClient.subscribeEvent();

    if (!imuSubToken || !baroSubToken || !fusedSensorDataSubToken || !flightStateSubToken || !eventSubToken) {
        printf("Logger Task: Failed to subscribe to all required topics!\n");
        vTaskDelete(NULL);
    }

    for (;;) {
        std::stringstream logStream;

        // Read and log IMU data
        SensorImu imuData;
        if (messagingClient.checkImu(imuSubToken.value())) {
            if (messagingClient.readImu(imuSubToken.value(), imuData)) {
                logStream << "IMU: Accel(" << imuData.accel_x << "," << imuData.accel_y << "," << imuData.accel_z << ") ";
            }
        }

        // Read and log Baro data
        SensorBaro baroData;
        if (messagingClient.checkBaro(baroSubToken.value())) {
            if (messagingClient.readBaro(baroSubToken.value(), baroData)) {
                logStream << "Baro: Pres(" << baroData.pressure_pa << ") Temp(" << baroData.temperature_c << ") Alt(" << baroData.altitude_m << ") ";
            }
        }

        // Read and log Fused Sensor data
        FusedSensorData fusedData;
        if (messagingClient.checkFusedSensorData(fusedSensorDataSubToken.value())) {
            if (messagingClient.readFusedSensorData(fusedSensorDataSubToken.value(), fusedData)) {
                // TODO: Uncomment when FusedSensorData is implemented
                //logStream << "Fused: Roll(" << fusedData.attitude_roll << ") Pitch(" << fusedData.attitude_pitch << ") Yaw(" << fusedData.attitude_yaw << ") Alt(" << fusedData.altitude_fused << ") VSpeed(" << fusedData.vertical_speed << ") ";
            }
        }

        // Read and log Flight State
        FlightState flightState;
        if (messagingClient.checkFlightState(flightStateSubToken.value())) {
            if (messagingClient.readFlightState(flightStateSubToken.value(), flightState)) {
                logStream << "State: " << static_cast<int>(flightState.current_phase) << " ";
            }
        }

        // Read and log Events
        Event event;
        if (messagingClient.checkEvent(eventSubToken.value())) {
            if (messagingClient.readEvent(eventSubToken.value(), event)) {
                logger.logEvent("Event: " + event.type);
            }
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