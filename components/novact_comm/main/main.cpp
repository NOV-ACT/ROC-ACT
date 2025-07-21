#include "ProtoSender.hpp"
#include "GcsStreamDriver.hpp"
#include "ICommunicationChannel.hpp"
#include <iostream>
#include <memory>

int main() {
    // --- Demonstrate with UART Channel ---
    std::cout << "--- Testing with UART Channel ---" << std::endl;
    auto uartChannel = std::make_unique<UartChannel>();
    GcsStreamDriver uartDriver(std::move(uartChannel));
    ProtoSender uartProtoSender(uartDriver);

    Telemetry telemetry_msg = Telemetry_init_zero;
    telemetry_msg.timestamp_ms = 1678886400000;
    telemetry_msg.latitude = 34.0522;
    telemetry_msg.longitude = -118.2437;
    telemetry_msg.altitude_m = 71.0;
    telemetry_msg.battery_voltage = 3.8;

    if (uartProtoSender.send(telemetry_msg)) {
        std::cout << "Successfully sent telemetry via UART." << std::endl;
    } else {
        std::cerr << "Failed to send telemetry via UART." << std::endl;
    }

    std::cout << std::endl;

    // --- Demonstrate with TCP Channel ---
    std::cout << "--- Testing with TCP Channel ---" << std::endl;
    auto tcpChannel = std::make_unique<TcpChannel>();
    GcsStreamDriver tcpDriver(std::move(tcpChannel));
    ProtoSender tcpProtoSender(tcpDriver);

    if (tcpProtoSender.send(telemetry_msg)) {
        std::cout << "Successfully sent telemetry via TCP." << std::endl;
    } else {
        std::cerr << "Failed to send telemetry via TCP." << std::endl;
    }

    return 0;
}