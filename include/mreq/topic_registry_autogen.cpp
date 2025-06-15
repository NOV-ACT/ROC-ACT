#include "topic_registry_autogen.hpp"
#include <sensor_imu.pb.h>
#include <sensor_baro.pb.h>
#include <flight_state.pb.h>
#include <pyro_command.pb.h>
#include <event.pb.h>

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
