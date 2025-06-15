#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>

// Forward declarations for tasks
extern "C" void imu_task(void* pvParameters);
extern "C" void baro_task(void* pvParameters);
extern "C" void state_task(void* pvParameters);
extern "C" void pyro_task(void* pvParameters);
extern "C" void logger_task(void* pvParameters);
extern "C" void telemetry_task(void* pvParameters);

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // This function will be called if a task overflows its stack.
    // You can add custom error handling or logging here.
    printf("ERROR: Stack overflow in task: %s\n", pcTaskName);
    while(1); // Halt the system
}

int main() {
    // Create tasks
    xTaskCreate(imu_task, "IMU_Task", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(baro_task, "BARO_Task", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(state_task, "STATE_Task", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY + 3, NULL);
    xTaskCreate(pyro_task, "PYRO_Task", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(logger_task, "LOGGER_Task", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(telemetry_task, "TELEMETRY_Task", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY + 1, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // Should never reach here
    for (;;);
    return 0;
}