#pragma once
#include <string>
#include "mreq/interface.hpp"

namespace mreq {
namespace autogen {

// Topic registrations
REGISTER_TOPIC(SensorImu, "sensor_imu")
REGISTER_TOPIC(SensorBaro, "sensor_baro")
REGISTER_TOPIC(FlightState, "flight_state")
REGISTER_TOPIC(PyroCommand, "pyro_command")
REGISTER_TOPIC(Event, "event")

} // namespace autogen
} // namespace mreq
