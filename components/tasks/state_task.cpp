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
    novact::core::MessagingClient messagingClient;
    // novact::core::SensorFusion sensorFusion(messagingClient); // TODO: Refactor SensorFusion to not need drivers

    printf("State Task: Initializing...\n");

    // Subscribe to topics
    // No need to subscribe to flight_state, as this task is the primary publisher.
    // Subscribing could be useful for a watchdog or redundant system in the future.

    for (;;) {
        // In a real implementation, SensorFusion would be called here to get the latest data
        // For now, we assume StateManager gets its data internally from the messaging client
        // sensorFusion.update();

        // Execute FSM logic and get the new state
        FlightPhase old_phase = stateManager.getCurrentState();
        FlightPhase new_phase = stateManager.updateState(messagingClient); // Pass client to updateState

        // Publish the new state if it has changed
        if (new_phase != old_phase) {
            FlightState newFlightState = FlightState_init_zero;
            newFlightState.timestamp_us = xTaskGetTickCount() * portTICK_PERIOD_MS;
            newFlightState.current_phase = new_phase;
            messagingClient.publishFlightState(newFlightState);

            // Publish an event for the state change
        }

        // Check if pyro channels should be fired
        stateManager.checkAndFirePyros(messagingClient);

        vTaskDelay(pdMS_TO_TICKS(20)); // FSM should run at a high frequency
    }
}