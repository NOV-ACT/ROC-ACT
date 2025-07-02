#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "drivers/BarometerDriver.hpp"
#include "core/MessagingClient.hpp"
#include "sensor_baro.pb.h" // For SensorBaro message

extern "C" void baro_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    novact::drivers::BarometerDriver barometerDriver;
    novact::core::MessagingClient messagingClient;

    if (!barometerDriver.initialize()) {
        printf("Baro Task: Failed to initialize Barometer Driver!\n");
        vTaskDelete(NULL); // Delete task if initialization fails
    }

    printf("Baro Task: Initializing...\n");

    for (;;) {
        novact::drivers::BarometerData baroRawData = barometerDriver.readData();

        novact::core::SensorBaro baroMessage;
        baroMessage.timestamp = xTaskGetTickCount(); // Use FreeRTOS tick count as timestamp
        baroMessage.pressure = baroRawData.pressure;
        baroMessage.temperature = baroRawData.temperature;
        baroMessage.altitude = baroRawData.altitude;

        messagingClient.publishBaro(baroMessage);

        printf("Baro Task: Reading and publishing baro data.\n");
        vTaskDelay(pdMS_TO_TICKS(200)); // Run every 200ms
    }
}