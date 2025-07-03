#include "MessagingClient.hpp"
#include <iostream> // For logging/debugging, replace with actual logging system
#include <optional> // Required for std::optional

#include "mreq/topic_registry.hpp"
#include "event.pb.h"
#include "flight_state.pb.h"
#include "pyro_command.pb.h"
#include "sensor_baro.pb.h"
#include "sensor_imu.pb.h"
#include "fused_sensor_data.pb.h" // Assuming this is also a generated proto

// MREQ integration for publish/subscribe mechanisms

namespace novact {
namespace core {

MessagingClient::MessagingClient()
    : imuTopic(TopicRegistry::instance().get_topic<SensorImu>("sensor_imu")),
      baroTopic(TopicRegistry::instance().get_topic<SensorBaro>("sensor_baro")),
      fusedSensorDataTopic(TopicRegistry::instance().get_topic<FusedSensorData>("fused_sensor_data")), // New topic
      flightStateTopic(TopicRegistry::instance().get_topic<FlightState>("flight_state")),
      pyroCommandTopic(TopicRegistry::instance().get_topic<PyroCommand>("pyro_command")),
      eventTopic(TopicRegistry::instance().get_topic<Event>("event")) {

    std::cout << "MessagingClient initialized. Subscribing to topics..." << std::endl;

    // Subscribe to topics and store tokens
    imuSubToken = imuTopic.subscribe();
    baroSubToken = baroTopic.subscribe();
    fusedSensorDataSubToken = fusedSensorDataTopic.subscribe(); // New subscription
    flightStateSubToken = flightStateTopic.subscribe();
    pyroCommandSubToken = pyroCommandTopic.subscribe();
    eventSubToken = eventTopic.subscribe();

    if (!imuSubToken || !baroSubToken || !fusedSensorDataSubToken || !flightStateSubToken || !pyroCommandSubToken || !eventSubToken) {
        std::cerr << "ERROR: Failed to subscribe to all MREQ topics!" << std::endl;
        // In a real system, this would trigger a more robust error handling mechanism
    }
}

bool MessagingClient::publishImu(const SensorImu& msg) {
    imuTopic.publish(msg);
    return true;
}

bool MessagingClient::publishBaro(const SensorBaro& msg) {
    baroTopic.publish(msg);
    return true;
}

bool MessagingClient::publishFusedSensorData(const FusedSensorData& msg) { // New publish method
    fusedSensorDataTopic.publish(msg);
    return true;
}

bool MessagingClient::publishFlightState(const FlightState& msg) {
    flightStateTopic.publish(msg);
    return true;
}

bool MessagingClient::publishPyroCommand(const PyroCommand& msg) {
    pyroCommandTopic.publish(msg);
    return true;
}

bool MessagingClient::publishEvent(const Event& msg) {
    eventTopic.publish(msg);
    return true;
}

std::optional<SensorImu> MessagingClient::readImu() {
    if (imuSubToken) {
        return imuTopic.read(imuSubToken.value());
    }
    return std::nullopt;
}

std::optional<SensorBaro> MessagingClient::readBaro() {
    if (baroSubToken) {
        return baroTopic.read(baroSubToken.value());
    }
    return std::nullopt;
}

std::optional<FusedSensorData> MessagingClient::readFusedSensorData() { // New read method
    if (fusedSensorDataSubToken) {
        return fusedSensorDataTopic.read(fusedSensorDataSubToken.value());
    }
    return std::nullopt;
}

std::optional<FlightState> MessagingClient::readFlightState() {
    if (flightStateSubToken) {
        return flightStateTopic.read(flightStateSubToken.value());
    }
    return std::nullopt;
}

std::optional<PyroCommand> MessagingClient::readPyroCommand() {
    if (pyroCommandSubToken) {
        return pyroCommandTopic.read(pyroCommandSubToken.value());
    }
    return std::nullopt;
}

std::optional<Event> MessagingClient::readEvent() {
    if (eventSubToken) {
        return eventTopic.read(eventSubToken.value());
    }
    return std::nullopt;
}

} // namespace core
} // namespace novact