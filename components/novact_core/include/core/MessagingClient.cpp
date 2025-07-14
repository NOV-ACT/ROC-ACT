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

    std::cout << "MessagingClient initialized." << std::endl;
    // No internal subscriptions needed anymore, external callers will manage their tokens.
}

// Publish methods
bool MessagingClient::publishImu(const SensorImu& msg) {
    imuTopic.publish(msg);
    return true;
}

bool MessagingClient::publishBaro(const SensorBaro& msg) {
    baroTopic.publish(msg);
    return true;
}

bool MessagingClient::publishFusedSensorData(const FusedSensorData& msg) {
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

// Subscribe methods
std::optional<size_t> MessagingClient::subscribeImu() {
    return imuTopic.subscribe();
}

std::optional<size_t> MessagingClient::subscribeBaro() {
    return baroTopic.subscribe();
}

std::optional<size_t> MessagingClient::subscribeFusedSensorData() {
    return fusedSensorDataTopic.subscribe();
}

std::optional<size_t> MessagingClient::subscribeFlightState() {
    return flightStateTopic.subscribe();
}

std::optional<size_t> MessagingClient::subscribePyroCommand() {
    return pyroCommandTopic.subscribe();
}

std::optional<size_t> MessagingClient::subscribeEvent() {
    return eventTopic.subscribe();
}

// Check methods
bool MessagingClient::checkImu(size_t token) {
    return imuTopic.check(token);
}

bool MessagingClient::checkBaro(size_t token) {
    return baroTopic.check(token);
}

bool MessagingClient::checkFusedSensorData(size_t token) {
    return fusedSensorDataTopic.check(token);
}

bool MessagingClient::checkFlightState(size_t token) {
    return flightStateTopic.check(token);
}

bool MessagingClient::checkPyroCommand(size_t token) {
    return pyroCommandTopic.check(token);
}

bool MessagingClient::checkEvent(size_t token) {
    return eventTopic.check(token);
}

// Read methods
bool MessagingClient::readImu(size_t token, SensorImu& out_msg) {
    std::optional<SensorImu> data = imuTopic.read(token);
    if (data) {
        out_msg = data.value();
        return true;
    }
    return false;
}

bool MessagingClient::readBaro(size_t token, SensorBaro& out_msg) {
    std::optional<SensorBaro> data = baroTopic.read(token);
    if (data) {
        out_msg = data.value();
        return true;
    }
    return false;
}

bool MessagingClient::readFusedSensorData(size_t token, FusedSensorData& out_msg) {
    std::optional<FusedSensorData> data = fusedSensorDataTopic.read(token);
    if (data) {
        out_msg = data.value();
        return true;
    }
    return false;
}

bool MessagingClient::readFlightState(size_t token, FlightState& out_msg) {
    std::optional<FlightState> data = flightStateTopic.read(token);
    if (data) {
        out_msg = data.value();
        return true;
    }
    return false;
}

bool MessagingClient::readPyroCommand(size_t token, PyroCommand& out_msg) {
    std::optional<PyroCommand> data = pyroCommandTopic.read(token);
    if (data) {
        out_msg = data.value();
        return true;
    }
    return false;
}

bool MessagingClient::readEvent(size_t token, Event& out_msg) {
    std::optional<Event> data = eventTopic.read(token);
    if (data) {
        out_msg = data.value();
        return true;
    }
    return false;
}

// Unsubscribe methods
void MessagingClient::unsubscribeImu(size_t token) {
    imuTopic.unsubscribe(token);
}

void MessagingClient::unsubscribeBaro(size_t token) {
    baroTopic.unsubscribe(token);
}

void MessagingClient::unsubscribeFusedSensorData(size_t token) {
    fusedSensorDataTopic.unsubscribe(token);
}

void MessagingClient::unsubscribeFlightState(size_t token) {
    flightStateTopic.unsubscribe(token);
}

void MessagingClient::unsubscribePyroCommand(size_t token) {
    pyroCommandTopic.unsubscribe(token);
}

void MessagingClient::unsubscribeEvent(size_t token) {
    eventTopic.unsubscribe(token);
}

} // namespace core
} // namespace novact