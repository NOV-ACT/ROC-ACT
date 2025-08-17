#ifndef LED_STATUS_CODES_HPP
#define LED_STATUS_CODES_HPP

/**
 * @brief Defines the different states of the system to be indicated by the RGB LED.
 */
enum class LedState {
    BOOTING,              // System is starting up
    WIFI_CONNECTING,      // Actively trying to connect to Wi-Fi
    WIFI_CONNECTED,       // Successfully connected to Wi-Fi
    OPERATION_IN_PROGRESS,// A generic operation is in progress (e.g., sensor calibration, data logging)
    ERROR,                // An error has occurred
    IDLE                  // System is in a normal, idle state
};

#endif // LED_STATUS_CODES_HPP