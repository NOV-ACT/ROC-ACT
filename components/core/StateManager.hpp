#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <cstdint>

namespace novact {
namespace core {

enum class FlightState : uint8_t {
    IDLE,
    BOOST,
    COAST,
    DEPLOY,
    RECOVERY
};

class StateManager {
public:
    StateManager();
    void updateState();
    FlightState getCurrentState() const;

private:
    FlightState currentState;
};

} // namespace core
} // namespace novact

#endif // STATE_MANAGER_HPP