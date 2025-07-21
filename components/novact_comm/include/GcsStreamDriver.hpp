#pragma once

#include "ICommunicationChannel.hpp"
#include <memory>

/**
 * @brief Manages the low-level communication channel for sending raw data.
 */
class GcsStreamDriver {
public:
    /**
     * @brief Constructs a GcsStreamDriver.
     * @param channel A unique pointer to a communication channel implementation.
     */
    explicit GcsStreamDriver(std::unique_ptr<ICommunicationChannel> channel);

    /**
     * @brief Sends a raw buffer of data through the communication channel.
     * @param buffer Pointer to the data buffer.
     * @param size Size of the data in bytes.
     */
    void send_raw(const uint8_t* buffer, size_t size);

private:
    std::unique_ptr<ICommunicationChannel> _channel;
};