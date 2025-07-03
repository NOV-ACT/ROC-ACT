#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>

namespace novact {
namespace core {

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual bool log(const std::string& message) = 0;
    virtual bool logError(const std::string& errorMessage) = 0;
    virtual bool logEvent(const std::string& eventMessage) = 0;
};

} // namespace core
} // namespace novact

#endif // ILOGGER_HPP