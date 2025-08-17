#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "drivers/ImuDriver.hpp"
#include "mreq/mreq.hpp"
#include "topic_registry_autogen.hpp"
#include "sensor_imu.pb.h" // For SensorImu message

using namespace mreq::autogen;

extern "C" void imu_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    novact::drivers::ImuDriver imuDriver;

    if (!imuDriver.initialize()) {
        printf("IMU Task: Failed to initialize IMU Driver!\n");
        vTaskDelete(NULL); // Delete task if initialization fails
    }

    printf("IMU Task: Initializing...\n");

    for (;;) {
        novact::drivers::ImuData imuRawData = imuDriver.readData();

        SensorImu imuMessage;
        imuMessage.timestamp_us = xTaskGetTickCount(); // Use FreeRTOS tick count as timestamp
        imuMessage.accel_x = imuRawData.accel_x;
        imuMessage.accel_y = imuRawData.accel_y;
        imuMessage.accel_z = imuRawData.accel_z;
        imuMessage.gyro_x = imuRawData.gyro_x;
        imuMessage.gyro_y = imuRawData.gyro_y;
        imuMessage.gyro_z = imuRawData.gyro_z;

        MREQ_PUBLISH(sensor_imu, imuMessage);

        printf("IMU Task: Reading and publishing IMU data.\n");
        vTaskDelay(pdMS_TO_TICKS(100)); // Run every 100ms
    }
}