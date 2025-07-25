#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "core/MessagingClient.hpp"
#include "drivers/TelemetryDriver.hpp"
#include "UnitCommProtocol.hpp" // Include UnitCommProtocol
#include "fused_sensor_data.pb.h" // For FusedSensorData
#include "flight_state.pb.h" // For FlightState
#include "pb_encode.h"
#include "sensor_imu.pb.h"
#include "sensor_baro.pb.h"
#include "event.pb.h"

// Cihaz ID ve numaraları için enumlar
enum class DeviceId : uint16_t {
    GCS         = 0x1001,
    FLIGHT_CTRL = 0x2002,
};

enum class DeviceNo : uint16_t {
    MAIN = 0x0001,
};

// Helper function to encode and transmit telemetry data
void transmit_telemetry_data(
    novact::drivers::TelemetryDriver& driver,
    UnitCommProtocol& protocol,
    const void* data,
    const pb_msgdesc_t* fields,
    uint8_t message_id,
    const char* log_message
) {
    printf("Telemetry Task: %s\n", log_message);
    uint8_t buffer[128]; // Increased buffer size for safety
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if (pb_encode(&stream, fields, data)) {
        std::vector<uint8_t> payload(buffer, buffer + stream.bytes_written);
        protocol.setHeader(UnitCommProtocol::PacketType::TELEMETRY, message_id,
            static_cast<uint16_t>(DeviceId::GCS), static_cast<uint16_t>(DeviceNo::MAIN),
            static_cast<uint16_t>(DeviceId::FLIGHT_CTRL), static_cast<uint16_t>(DeviceNo::MAIN));
        protocol.setPayload(payload);
        protocol.finalizeFrame();
        std::vector<uint8_t> frame = protocol.serializeFrame();
        driver.sendData(frame.data(), frame.size());
    }
}

extern "C" void telemetry_task(void* pvParameters) {
    (void)pvParameters; // Unused parameter

    // Task initialization (e.g., RF module setup)
    printf("Telemetry Task: Initializing...\n");

    novact::drivers::TelemetryDriver telemetryDriver;
    if (!telemetryDriver.initialize()) {
        printf("Telemetry Task: TelemetryDriver initialization failed!\n");
        vTaskDelete(NULL);
    }

    UnitCommProtocol telemetryProtocol; // Instance for sending telemetry
    UnitCommProtocol commandProtocol;   // Instance for receiving commands (example)

    novact::core::MessagingClient messagingClient;

    // Subscribe to flight_state and event topics
    std::optional<size_t> flightStateSubToken = messagingClient.subscribeFlightState();
    std::optional<size_t> eventSubToken = messagingClient.subscribeEvent();
    std::optional<size_t> imuSubToken = messagingClient.subscribeImu();
    std::optional<size_t> baroSubToken = messagingClient.subscribeBaro();
    std::optional<size_t> fusedSensorDataSubToken = messagingClient.subscribeFusedSensorData();

    if (!flightStateSubToken || !eventSubToken || !imuSubToken || !baroSubToken || !fusedSensorDataSubToken) {
        printf("Telemetry Task: Failed to subscribe to all required topics!\n");
        vTaskDelete(NULL);
    }

    for (;;) {
        // Read and transmit Flight State
        FlightState flightState;
        if (messagingClient.readFlightState(flightStateSubToken.value(), flightState)) {
            char log_msg[64];
            snprintf(log_msg, sizeof(log_msg), "Transmitting Flight State: %d", static_cast<int>(flightState.current_phase));
            transmit_telemetry_data(telemetryDriver, telemetryProtocol, &flightState, FlightState_fields, 0x01, log_msg);
        }

        // Read and transmit Events
        Event event;
        if (messagingClient.readEvent(eventSubToken.value(), event)) {
            char log_msg[64];
            snprintf(log_msg, sizeof(log_msg), "Transmitting Event: %d", static_cast<int>(event.type));
            transmit_telemetry_data(telemetryDriver, telemetryProtocol, &event, Event_fields, 0x02, log_msg);
        }

        // Read and transmit IMU data
        SensorImu imuData;
        if (messagingClient.readImu(imuSubToken.value(), imuData)) {
            transmit_telemetry_data(telemetryDriver, telemetryProtocol, &imuData, SensorImu_fields, 0x03, "Transmitting IMU data.");
        }

        // Read and transmit Baro data
        SensorBaro baroData;
        if (messagingClient.readBaro(baroSubToken.value(), baroData)) {
            transmit_telemetry_data(telemetryDriver, telemetryProtocol, &baroData, SensorBaro_fields, 0x04, "Transmitting Baro data.");
        }

        // Read and transmit Fused Sensor data using the new protocol
        FusedSensorData fusedData;
        if (messagingClient.readFusedSensorData(fusedSensorDataSubToken.value(), fusedData)) {
            transmit_telemetry_data(telemetryDriver, telemetryProtocol, &fusedData, FusedSensorData_fields, 0x05, "Transmitting Fused Sensor data.");
        }

        // Placeholder for receiving and parsing incoming data
        std::vector<uint8_t> received_raw_bytes = telemetryDriver.receiveData();
                if (commandProtocol.deserializeFrame(received_raw_bytes)) {
                    if (commandProtocol.isValid()) {
                        printf("Telemetry Task: Received and parsed valid incoming frame. Packet Type: %d\n", (int)commandProtocol.getPacketType());
                        // Example: Dispatch based on packet type
                        // if (commandProtocol.getPacketType() == static_cast<uint8_t>(UnitCommProtocol::PacketType::COMMAND)) {
                        //     CommandData command;
                        //     if (commandProtocol.getParsedPayload(command)) {
                        //         // Process command
                        //     }
                        // }
                        commandProtocol.resetParser(); // Reset parser after a complete frame
                    } else {
                        printf("Telemetry Task: Received incoming frame is invalid.\n");
                        commandProtocol.resetParser(); // Reset parser on invalid frame
                    }
                }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Run every 1000ms
    }
}