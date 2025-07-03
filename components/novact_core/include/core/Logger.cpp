#include "Logger.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "drivers/SDCardDriver.hpp"

namespace novact {
namespace core {

Logger::Logger(drivers::SDCardDriver& sdCardDriver) : sdCard(sdCardDriver) {
    // Logger initialized, SD card driver reference stored.
}

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool Logger::log(const std::string& message) {
    std::string logEntry = getCurrentTimestamp() + " [INFO] " + message;
    return sdCard.writeLog(logEntry);
}

bool Logger::logError(const std::string& errorMessage) {
    std::string logEntry = getCurrentTimestamp() + " [ERROR] " + errorMessage;
    return sdCard.writeLog(logEntry);
}

bool Logger::logEvent(const std::string& eventMessage) {
    std::string logEntry = getCurrentTimestamp() + " [EVENT] " + eventMessage;
    return sdCard.writeLog(logEntry);
}

} // namespace core
} // namespace novact