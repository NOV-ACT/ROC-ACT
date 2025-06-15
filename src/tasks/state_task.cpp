#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>

extern "C" void state_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    // Task initialization (e.g., FSM setup)
    printf("State Task: Initializing...\n");

    for (;;) {
        // Read sensor data from topics
        // Execute FSM logic
        // Publish flight_state, pyro_command, event topics
        printf("State Task: Managing flight state.\n");
        vTaskDelay(pdMS_TO_TICKS(50)); // Run every 50ms
    }
}