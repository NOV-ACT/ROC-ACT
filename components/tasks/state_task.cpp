#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "core/StateManager.hpp"
#include "core/SensorFusion.hpp"
#include "core/MessagingClient.hpp"
#include "drivers/ImuDriver.hpp"     // Required for SensorFusion instantiation
#include "drivers/BarometerDriver.hpp" // Required for SensorFusion instantiation
#include "fused_sensor_data.pb.h"
#include "flight_state.pb.h"
#include "pyro_command.pb.h"
#include "event.pb.h"

extern "C" void state_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    novact::core::StateManager stateManager;
    novact::drivers::ImuDriver imuDriver; // Instantiate for SensorFusion
    novact::drivers::BarometerDriver baroDriver; // Instantiate for SensorFusion
    novact::core::MessagingClient messagingClient;
    novact::core::SensorFusion sensorFusion(imuDriver, baroDriver, messagingClient);

    printf("State Task: Initializing...\n");

    // Initialize drivers (though they are initialized in their own tasks,
    // SensorFusion needs initialized drivers to read data)
    if (!imuDriver.initialize() || !baroDriver.initialize()) {
        printf("State Task: Failed to initialize sensor drivers for SensorFusion!\n");
        vTaskDelete(NULL);
    }

    // Subscribe to topics
    std::optional<size_t> flightStateSubToken = messagingClient.subscribeFlightState();
    if (!flightStateSubToken) {
        printf("State Task: Failed to subscribe to flight_state topic!\n");
        vTaskDelete(NULL);
    }

    for (;;) {
        // Update and publish fused sensor data
        //novact::core::FusedSensorData fusedData = sensorFusion.updateAndPublish();

        // Read latest flight state from topic (if published by another task or for redundancy)
        FlightState currentFlightState;
        if (messagingClient.checkFlightState(flightStateSubToken.value())) {
            if (messagingClient.readFlightState(flightStateSubToken.value(), currentFlightState)) {
                // Potentially use this for state validation or external control
                // For now, StateManager will drive the state
            }
        }

        // Execute FSM logic
        // stateManager.updateState();
        // novact::core::FlightState currentFSMState;
        // currentFSMState.timestamp = xTaskGetTickCount();
        // currentFSMState.state = static_cast<uint8_t>(stateManager.getCurrentState());
        // messagingClient.publishFlightState(currentFSMState);

        // Placeholder for pyro command and event publishing based on FSM state
        // if (stateManager.getCurrentState() == novact::core::FlightState::DEPLOY) {
        //     novact::core::PyroCommand deployCommand;
        //     deployCommand.timestamp = xTaskGetTickCount();
        //     deployCommand.channel = 1; // Main parachute
        //     deployCommand.activate = true;
        //     messagingClient.publishPyroCommand(deployCommand);

        //     novact::core::Event deployEvent;
        //     deployEvent.timestamp = xTaskGetTickCount();
        //     deployEvent.event_id = 101; // Example event ID
        //     deployEvent.description = "Main parachute deployed.";
        //     messagingClient.publishEvent(deployEvent);
        // }

        printf("State Task: Managing flight state. Current FSM State: %d\n", static_cast<int>(stateManager.getCurrentState()));
        vTaskDelay(pdMS_TO_TICKS(50)); // Run every 50ms
    }
}