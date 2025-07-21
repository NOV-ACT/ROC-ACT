#pragma once

#include "GcsStreamDriver.hpp"
#include "pb_encode.h"
#include <vector>
#include <iostream>

class ProtoSender {
public:
    explicit ProtoSender(GcsStreamDriver& driver);

    template<typename T>
    bool send(const pb_field_t fields[], const T* message)
    {
        size_t required_size;
        if (!pb_get_encoded_size(&required_size, fields, message)) {
            std::cerr << "Failed to get encoded size for message" << std::endl;
            return false;
        }

        std::vector<uint8_t> buffer(required_size);
        pb_ostream_t stream = pb_ostream_from_buffer(buffer.data(), buffer.size());

        if (!pb_encode(&stream, fields, message)) {
            std::cerr << "Failed to encode message: " << PB_GET_ERROR(&stream) << std::endl;
            return false;
        }

        _driver.send_raw(buffer.data(), stream.bytes_written);
        return true;
    }

private:
    GcsStreamDriver& _driver;
};