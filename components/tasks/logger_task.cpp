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

// Helper function to write the CSV header for the log file
void write_log_header(novact::core::Logger& logger) {
    const char* header = "timestamp_ms,accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z,pressure_pa,temperature_c,altitude_m,flight_phase,event_type,event_data";
    logger.log(header);
}

extern "C" void logger_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    novact::drivers::SDCardDriver sdCardDriver;
    novact::core::Logger logger(sdCardDriver);
    novact::core::MessagingClient messagingClient;

    if (!sdCardDriver.initialize()) {
        printf("Logger Task: Failed to initialize SD Card Driver!\n");
        vTaskDelete(NULL); // Delete task if initialization fails
    }

    printf("Logger Task: Initializing and writing header...\n");
    write_log_header(logger);

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
        std::stringstream csv_line;

        // Timestamp
        csv_line << xTaskGetTickCount() * portTICK_PERIOD_MS << ",";

        // Read and log IMU data
        SensorImu imuData;
        if (messagingClient.readImu(imuSubToken.value(), imuData)) {
            csv_line << imuData.accel_x << "," << imuData.accel_y << "," << imuData.accel_z << ","
                     << imuData.gyro_x << "," << imuData.gyro_y << "," << imuData.gyro_z;
        }
        csv_line << ",";

        // Read and log Baro data
        SensorBaro baroData;
        if (messagingClient.readBaro(baroSubToken.value(), baroData)) {
            csv_line << baroData.pressure_pa << "," << baroData.temperature_c << "," << baroData.altitude_m;
        }
        csv_line << ",";

        // Read and log Flight State
        FlightState flightState;
        if (messagingClient.readFlightState(flightStateSubToken.value(), flightState)) {
            // csv_line << static_cast<int>(flightState.current_phase);
        }
        csv_line << ",";

        // Read and log Events
        Event event;
        if (messagingClient.readEvent(eventSubToken.value(), event)) {
            // csv_line << static_cast<int>(event.type) << "," << event.message;
        }
        // No final comma needed for the last field

        std::string logEntry = csv_line.str();
        
        // Log only if the line is not empty or just a timestamp with commas
        if (logEntry.length() > 25) { // Heuristic check to avoid nearly empty lines
             logger.log(logEntry);
             // printf("Logger Task: Logged: %s\n", logEntry.c_str()); // Optional: for debugging
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Log more frequently for higher resolution data
    }
}