#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>

extern "C" void baro_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    // Task initialization (e.g., Barometer sensor setup)
    printf("Baro Task: Initializing...\n");

    for (;;) {
        // Read Barometer data
        // Estimate altitude
        // Publish to sensor_baro topic
        printf("Baro Task: Reading and publishing baro data.\n");
        vTaskDelay(pdMS_TO_TICKS(200)); // Run every 200ms
    }
}