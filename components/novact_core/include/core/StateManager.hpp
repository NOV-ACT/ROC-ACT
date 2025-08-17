#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include "flight_state.pb.h"
#include <cstdint>

namespace novact {
namespace core {

class StateManager {
public:
    StateManager();

    /**
     * @brief Updates the flight state based on sensor data from topics.
     * @return The new flight state.
     */
    FlightPhase updateState();

    /**
     * @brief Checks if any pyrotechnic channels should be fired based on the current state and conditions.
     */
    void checkAndFirePyros();

    /**
     * @brief Gets the current flight state.
     * @return The current flight state.
     */
    FlightPhase getCurrentState() const;

private:
    FlightPhase currentState;
    
    // State transition logic variables
    uint32_t launch_time_ms = 0;
    uint32_t coast_start_time_ms = 0;
    float max_altitude = 0.0f;
    float last_altitude = 0.0f;
    int consecutive_descent_readings = 0;

    // Pyro firing flags
    bool main_pyro_fired = false;
    bool drogue_pyro_fired = false;
};

} // namespace core
} // namespace novact

#endif // STATE_MANAGER_HPP