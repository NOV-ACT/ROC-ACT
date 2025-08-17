#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "drivers/BarometerDriver.hpp"
#include "mreq/mreq.hpp"
#include "topic_registry_autogen.hpp"
#include "sensor_baro.pb.h" // For SensorBaro message

using namespace mreq::autogen;

extern "C" void baro_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    novact::drivers::BarometerDriver barometerDriver;

    if (!barometerDriver.initialize()) {
        printf("Baro Task: Failed to initialize Barometer Driver!\n");
        vTaskDelete(NULL); // Delete task if initialization fails
    }

    printf("Baro Task: Initializing...\n");

    for (;;) {
        novact::drivers::BarometerData baroRawData = barometerDriver.readData();

        SensorBaro baroMessage;
        baroMessage.timestamp_us = xTaskGetTickCount(); // Use FreeRTOS tick count as timestamp
        baroMessage.pressure_pa = baroRawData.pressure;
        baroMessage.temperature_c = baroRawData.temperature;
        baroMessage.altitude_m = baroRawData.altitude;

        MREQ_PUBLISH(sensor_baro, baroMessage);

        printf("Baro Task: Reading and publishing baro data.\n");
        vTaskDelay(pdMS_TO_TICKS(200)); // Run every 200ms
    }
}