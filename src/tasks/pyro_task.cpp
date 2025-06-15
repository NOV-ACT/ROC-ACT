#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>

extern "C" void pyro_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    // Task initialization (e.g., GPIO setup for pyro channels)
    printf("Pyro Task: Initializing...\n");

    for (;;) {
        // Subscribe to pyro_command topic
        // Apply commands to GPIO outputs
        printf("Pyro Task: Processing pyro commands.\n");
        vTaskDelay(pdMS_TO_TICKS(10)); // Run every 10ms (high priority)
    }
}