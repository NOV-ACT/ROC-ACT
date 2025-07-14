#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

extern "C" void telemetry_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    // Task initialization (e.g., RF module setup)
    printf("Telemetry Task: Initializing...\n");

    novact::core::MessagingClient messagingClient;

    // Subscribe to flight_state and event topics
    std::optional<size_t> flightStateSubToken = messagingClient.subscribeFlightState();
    std::optional<size_t> eventSubToken = messagingClient.subscribeEvent();
    std::optional<size_t> imuSubToken = messagingClient.subscribeImu();
    std::optional<size_t> baroSubToken = messagingClient.subscribeBaro();
    std::optional<size_t> fusedSensorDataSubToken = messagingClient.subscribeFusedSensorData();

    if (!flightStateSubToken || !eventSubToken || !imuSubToken || !baroSubToken || !fusedSensorDataSubToken) {
        printf("Telemetry Task: Failed to subscribe to all required topics!\n");
        vTaskDelete(NULL);
    }

    for (;;) {
        // Read and transmit Flight State
        FlightState flightState;
        if (messagingClient.checkFlightState(flightStateSubToken.value())) {
            if (messagingClient.readFlightState(flightStateSubToken.value(), flightState)) {
                printf("Telemetry Task: Transmitting Flight State: %d\n", static_cast<int>(flightState.current_phase));
                // Transmit flightState via RF
            }
        }

        // Read and transmit Events
        Event event;
        if (messagingClient.checkEvent(eventSubToken.value())) {
            if (messagingClient.readEvent(eventSubToken.value(), event)) {
                printf("Telemetry Task: Transmitting Event: %s\n", event.type.c_str());
                // Transmit event via RF
            }
        }

        // Read and transmit IMU data
        SensorImu imuData;
        if (messagingClient.checkImu(imuSubToken.value())) {
            if (messagingClient.readImu(imuSubToken.value(), imuData)) {
                printf("Telemetry Task: Transmitting IMU data.\n");
                // Transmit imuData via RF
            }
        }

        // Read and transmit Baro data
        SensorBaro baroData;
        if (messagingClient.checkBaro(baroSubToken.value())) {
            if (messagingClient.readBaro(baroSubToken.value(), baroData)) {
                printf("Telemetry Task: Transmitting Baro data.\n");
                // Transmit baroData via RF
            }
        }

        // Read and transmit Fused Sensor data
        FusedSensorData fusedData;
        if (messagingClient.checkFusedSensorData(fusedSensorDataSubToken.value())) {
            if (messagingClient.readFusedSensorData(fusedSensorDataSubToken.value(), fusedData)) {
                printf("Telemetry Task: Transmitting Fused Sensor data.\n");
                // Transmit fusedData via RF
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Run every 1000ms
    }
}