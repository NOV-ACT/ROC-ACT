#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>

extern "C" void imu_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    // Task initialization (e.g., IMU sensor setup)
    printf("IMU Task: Initializing...\n");

    for (;;) {
        // Read IMU data
        // Publish to sensor_imu topic
        printf("IMU Task: Reading and publishing IMU data.\n");
        vTaskDelay(pdMS_TO_TICKS(100)); // Run every 100ms
    }
}