#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "ILogger.hpp"
#include "drivers/SDCardDriver.hpp" // Include the concrete driver

namespace novact {
namespace core {

class Logger : public ILogger {
public:
    explicit Logger(drivers::SDCardDriver& sdCardDriver);
    bool log(const std::string& message) override;
    bool logError(const std::string& errorMessage) override;
    bool logEvent(const std::string& eventMessage) override;

private:
    drivers::SDCardDriver& sdCard;
};

} // namespace core
} // namespace novact

#endif // LOGGER_HPP