#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>

extern "C" void telemetry_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    // Task initialization (e.g., RF module setup)
    printf("Telemetry Task: Initializing...\n");

    for (;;) {
        // Subscribe to flight_state, event topics
        // Transmit data via RF
        printf("Telemetry Task: Transmitting telemetry data.\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Run every 1000ms
    }
}