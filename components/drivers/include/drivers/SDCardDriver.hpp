#ifndef SDCARD_DRIVER_HPP
#define SDCARD_DRIVER_HPP

#include <string>
#include <cstdint>

namespace novact {
namespace drivers {

class SDCardDriver {
public:
    SDCardDriver();
    bool initialize();
    bool writeLog(const std::string& data);
    // Potentially add read functions, file management etc.

private:
    // Placeholder for hardware specific members (e.g., SPI handle, file system object)
};

} // namespace drivers
} // namespace novact

#endif // SDCARD_DRIVER_HPP