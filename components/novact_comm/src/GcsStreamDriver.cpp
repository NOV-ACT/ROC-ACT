#include "GcsStreamDriver.hpp"
#include <utility>

GcsStreamDriver::GcsStreamDriver(std::unique_ptr<ICommunicationChannel> channel)
    : _channel(std::move(channel)) {}

void GcsStreamDriver::send_raw(const uint8_t* buffer, size_t size) {
    if (_channel) {
        _channel->send(buffer, size);
    }
}