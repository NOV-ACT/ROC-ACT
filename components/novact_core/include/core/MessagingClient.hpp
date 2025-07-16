#ifndef MESSAGING_CLIENT_HPP
#define MESSAGING_CLIENT_HPP

#include <string>
#include <cstdint>
#include <optional>           // Required for std::optional
#include <functional>         // Required for std::reference_wrapper
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

    // New subscribe methods: External callers get their own token
    std::optional<size_t> subscribeImu();
    std::optional<size_t> subscribeBaro();
    std::optional<size_t> subscribeFusedSensorData();
    std::optional<size_t> subscribeFlightState();
    std::optional<size_t> subscribePyroCommand();
    std::optional<size_t> subscribeEvent();

    // New check methods: External callers use their token to check for new data
    bool checkImu(size_t token);
    bool checkBaro(size_t token);
    bool checkFusedSensorData(size_t token);
    bool checkFlightState(size_t token);
    bool checkPyroCommand(size_t token);
    bool checkEvent(size_t token);

    // New read methods: External callers use their token to read data into an output parameter
    // Returns true on successful read (new data available), false otherwise.
    bool readImu(size_t token, SensorImu& out_msg);
    bool readBaro(size_t token, SensorBaro& out_msg);
    bool readFusedSensorData(size_t token, FusedSensorData& out_msg);
    bool readFlightState(size_t token, FlightState& out_msg);
    bool readPyroCommand(size_t token, PyroCommand& out_msg);
    bool readEvent(size_t token, Event& out_msg);

    // New unsubscribe methods: External callers release their token
    void unsubscribeImu(size_t token);
    void unsubscribeBaro(size_t token);
    void unsubscribeFusedSensorData(size_t token);
    void unsubscribeFlightState(size_t token);
    void unsubscribePyroCommand(size_t token);
    void unsubscribeEvent(size_t token);

private:
    // References to MREQ topics, obtained from TopicRegistry
    std::optional<std::reference_wrapper<Topic<SensorImu, 10>>> imuTopic;
    std::optional<std::reference_wrapper<Topic<SensorBaro, 10>>> baroTopic;
    std::optional<std::reference_wrapper<Topic<FusedSensorData, 10>>> fusedSensorDataTopic;
    std::optional<std::reference_wrapper<Topic<FlightState, 10>>> flightStateTopic;
    std::optional<std::reference_wrapper<Topic<PyroCommand, 10>>> pyroCommandTopic;
    std::optional<std::reference_wrapper<Topic<Event, 10>>> eventTopic;
};

} // namespace core
} // namespace novact

#endif // MESSAGING_CLIENT_HPP