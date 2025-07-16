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

MessagingClient::MessagingClient() {
    // Register topics first to ensure they exist
    imuTopic = std::make_optional(std::ref(TopicRegistry::instance().register_topic<SensorImu, 10>("sensor_imu")));
    baroTopic = std::make_optional(std::ref(TopicRegistry::instance().register_topic<SensorBaro, 10>("sensor_baro")));
    fusedSensorDataTopic = std::make_optional(std::ref(TopicRegistry::instance().register_topic<FusedSensorData, 10>("fused_sensor_data")));
    flightStateTopic = std::make_optional(std::ref(TopicRegistry::instance().register_topic<FlightState, 10>("flight_state")));
    pyroCommandTopic = std::make_optional(std::ref(TopicRegistry::instance().register_topic<PyroCommand, 10>("pyro_command")));
    eventTopic = std::make_optional(std::ref(TopicRegistry::instance().register_topic<Event, 10>("event")));

    // No internal subscriptions needed anymore, external callers will manage their tokens.
}

// Publish methods
bool MessagingClient::publishImu(const SensorImu& msg) {
    if (imuTopic) {
        imuTopic->get().publish(msg);
        return true;
    }
    return false;
}

bool MessagingClient::publishBaro(const SensorBaro& msg) {
    if (baroTopic) {
        baroTopic->get().publish(msg);
        return true;
    }
    return false;
}

bool MessagingClient::publishFusedSensorData(const FusedSensorData& msg) {
    if (fusedSensorDataTopic) {
        fusedSensorDataTopic->get().publish(msg);
        return true;
    }
    return false;
}

bool MessagingClient::publishFlightState(const FlightState& msg) {
    if (flightStateTopic) {
        flightStateTopic->get().publish(msg);
        return true;
    }
    return false;
}

bool MessagingClient::publishPyroCommand(const PyroCommand& msg) {
    if (pyroCommandTopic) {
        pyroCommandTopic->get().publish(msg);
        return true;
    }
    return false;
}

bool MessagingClient::publishEvent(const Event& msg) {
    if (eventTopic) {
        eventTopic->get().publish(msg);
        return true;
    }
    return false;
}

// Subscribe methods
std::optional<size_t> MessagingClient::subscribeImu() {
    if (imuTopic) {
        return imuTopic->get().subscribe();
    }
    return std::nullopt;
}

std::optional<size_t> MessagingClient::subscribeBaro() {
    if (baroTopic) {
        return baroTopic->get().subscribe();
    }
    return std::nullopt;
}

std::optional<size_t> MessagingClient::subscribeFusedSensorData() {
    if (fusedSensorDataTopic) {
        return fusedSensorDataTopic->get().subscribe();
    }
    return std::nullopt;
}

std::optional<size_t> MessagingClient::subscribeFlightState() {
    if (flightStateTopic) {
        return flightStateTopic->get().subscribe();
    }
    return std::nullopt;
}

std::optional<size_t> MessagingClient::subscribePyroCommand() {
    if (pyroCommandTopic) {
        return pyroCommandTopic->get().subscribe();
    }
    return std::nullopt;
}

std::optional<size_t> MessagingClient::subscribeEvent() {
    if (eventTopic) {
        return eventTopic->get().subscribe();
    }
    return std::nullopt;
}

// Check methods
bool MessagingClient::checkImu(size_t token) {
    if (imuTopic) {
        return imuTopic->get().check(token);
    }
    return false;
}

bool MessagingClient::checkBaro(size_t token) {
    if (baroTopic) {
        return baroTopic->get().check(token);
    }
    return false;
}

bool MessagingClient::checkFusedSensorData(size_t token) {
    if (fusedSensorDataTopic) {
        return fusedSensorDataTopic->get().check(token);
    }
    return false;
}

bool MessagingClient::checkFlightState(size_t token) {
    if (flightStateTopic) {
        return flightStateTopic->get().check(token);
    }
    return false;
}

bool MessagingClient::checkPyroCommand(size_t token) {
    if (pyroCommandTopic) {
        return pyroCommandTopic->get().check(token);
    }
    return false;
}

bool MessagingClient::checkEvent(size_t token) {
    if (eventTopic) {
        return eventTopic->get().check(token);
    }
    return false;
}

// Read methods
bool MessagingClient::readImu(size_t token, SensorImu& out_msg) {
    if (imuTopic) {
        std::optional<SensorImu> data = imuTopic->get().read(token);
        if (data) {
            out_msg = data.value();
            return true;
        }
    }
    return false;
}

bool MessagingClient::readBaro(size_t token, SensorBaro& out_msg) {
    if (baroTopic) {
        std::optional<SensorBaro> data = baroTopic->get().read(token);
        if (data) {
            out_msg = data.value();
            return true;
        }
    }
    return false;
}

bool MessagingClient::readFusedSensorData(size_t token, FusedSensorData& out_msg) {
    if (fusedSensorDataTopic) {
        std::optional<FusedSensorData> data = fusedSensorDataTopic->get().read(token);
        if (data) {
            out_msg = data.value();
            return true;
        }
    }
    return false;
}

bool MessagingClient::readFlightState(size_t token, FlightState& out_msg) {
    if (flightStateTopic) {
        std::optional<FlightState> data = flightStateTopic->get().read(token);
        if (data) {
            out_msg = data.value();
            return true;
        }
    }
    return false;
}

bool MessagingClient::readPyroCommand(size_t token, PyroCommand& out_msg) {
    if (pyroCommandTopic) {
        std::optional<PyroCommand> data = pyroCommandTopic->get().read(token);
        if (data) {
            out_msg = data.value();
            return true;
        }
    }
    return false;
}

bool MessagingClient::readEvent(size_t token, Event& out_msg) {
    if (eventTopic) {
        std::optional<Event> data = eventTopic->get().read(token);
        if (data) {
            out_msg = data.value();
            return true;
        }
    }
    return false;
}

// Unsubscribe methods
void MessagingClient::unsubscribeImu(size_t token) {
    if (imuTopic) {
        imuTopic->get().unsubscribe(token);
    }
}

void MessagingClient::unsubscribeBaro(size_t token) {
    if (baroTopic) {
        baroTopic->get().unsubscribe(token);
    }
}

void MessagingClient::unsubscribeFusedSensorData(size_t token) {
    if (fusedSensorDataTopic) {
        fusedSensorDataTopic->get().unsubscribe(token);
    }
}

void MessagingClient::unsubscribeFlightState(size_t token) {
    if (flightStateTopic) {
        flightStateTopic->get().unsubscribe(token);
    }
}

void MessagingClient::unsubscribePyroCommand(size_t token) {
    if (pyroCommandTopic) {
        pyroCommandTopic->get().unsubscribe(token);
    }
}

void MessagingClient::unsubscribeEvent(size_t token) {
    if (eventTopic) {
        eventTopic->get().unsubscribe(token);
    }
}

} // namespace core
} // namespace novact