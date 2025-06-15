#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>

extern "C" void logger_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    // Task initialization (e.g., SD card setup)
    printf("Logger Task: Initializing...\n");

    for (;;) {
        // Subscribe to sensor_*, flight_state, event topics
        // Log data to SD card
        printf("Logger Task: Logging flight data.\n");
        vTaskDelay(pdMS_TO_TICKS(500)); // Run every 500ms
    }
}