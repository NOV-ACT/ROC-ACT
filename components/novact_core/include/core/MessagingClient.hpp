#ifndef MESSAGING_CLIENT_HPP
#define MESSAGING_CLIENT_HPP

#include <string>
#include <cstdint>
#include <optional>           // Required for std::optional
#include "mreq/interface.hpp" // For TopicRegistry
#include "mreq/topic.hpp"     // For Topic<T>

#include "StateManager.hpp" // For FlightState enum
#include "event.pb.h"
#include "flight_state.pb.h"
#include "pyro_command.pb.h"
#include "sensor_baro.pb.h"
#include "sensor_imu.pb.h"
#include "fused_sensor_data.pb.h" // Assuming this is also a generated proto

namespace novact {
namespace core {




class MessagingClient {
public:
    MessagingClient();
    // Publish methods for specific message types
    bool publishImu(const SensorImu& msg);
    bool publishBaro(const SensorBaro& msg);
    bool publishFusedSensorData(const FusedSensorData& msg); // New publish method
    bool publishFlightState(const FlightState& msg);
    bool publishPyroCommand(const PyroCommand& msg);
    bool publishEvent(const Event& msg);

    // Subscribe methods (simplified for now, will expand if needed)
    // For demonstration, we'll use a polling approach.
    std::optional<SensorImu> readImu();
    std::optional<SensorBaro> readBaro();
    std::optional<FusedSensorData> readFusedSensorData(); // New read method
    std::optional<FlightState> readFlightState();
    std::optional<PyroCommand> readPyroCommand();
    std::optional<Event> readEvent();

private:
    // References to MREQ topics, obtained from TopicRegistry
    Topic<SensorImu>& imuTopic;
    Topic<SensorBaro>& baroTopic;
    Topic<FusedSensorData>& fusedSensorDataTopic; // New topic reference
    Topic<FlightState>& flightStateTopic;
    Topic<PyroCommand>& pyroCommandTopic;
    Topic<Event>& eventTopic;

    // Subscriber tokens for polling
    std::optional<size_t> imuSubToken;
    std::optional<size_t> baroSubToken;
    std::optional<size_t> fusedSensorDataSubToken; // New subscriber token
    std::optional<size_t> flightStateSubToken;
    std::optional<size_t> pyroCommandSubToken;
    std::optional<size_t> eventSubToken;
};

} // namespace core
} // namespace novact

#endif // MESSAGING_CLIENT_HPP