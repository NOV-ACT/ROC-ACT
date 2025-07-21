#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iomanip>

/**
 * @brief Pure virtual interface for a communication channel.
 */
class ICommunicationChannel {
public:
    virtual ~ICommunicationChannel() = default;
    /**
     * @brief Sends data over the communication channel.
     * @param data Pointer to the data buffer.
     * @param length Length of the data in bytes.
     * @return true if the data was sent successfully, false otherwise.
     */
    virtual bool send(const uint8_t* data, size_t length) = 0;
};

/**
 * @brief Mock implementation of a UART communication channel.
 */
class UartChannel : public ICommunicationChannel {
public:
    bool send(const uint8_t* data, size_t length) override {
        std::cout << "UART Channel: Sending " << length << " bytes: ";
        for (size_t i = 0; i < length; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        return true;
    }
};

/**
 * @brief Mock implementation of a TCP communication channel.
 */
class TcpChannel : public ICommunicationChannel {
public:
    bool send(const uint8_t* data, size_t length) override {
        std::cout << "TCP Channel: Sending " << length << " bytes: ";
        for (size_t i = 0; i < length; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        return true;
    }
};