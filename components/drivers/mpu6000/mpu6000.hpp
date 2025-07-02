#pragma once

#include <cstdint>
#include "hal/SPI.hpp"
#include "hal/I2C.hpp"

namespace PX4_Mpu6000 {

class MPU6000 {
public:
    enum class Interface {
        SPI,
        I2C
    };

    MPU6000(AP_HAL::SPI* spi_interface, uint8_t cs_pin);
    MPU6000(AP_HAL::I2C* i2c_interface, uint8_t address);

    bool init();
    bool probe();
    bool read_accel_gyro(float& accel_x, float& accel_y, float& accel_z,
                         float& gyro_x, float& gyro_y, float& gyro_z);

private:
    Interface _interface_type;
    AP_HAL::SPI* _spi;
    AP_HAL::I2C* _i2c;
    uint8_t _cs_pin;
    uint8_t _address;

    uint8_t read_reg(uint8_t reg);
    void write_reg(uint8_t reg, uint8_t value);
    bool read_registers(uint8_t reg, uint8_t *data, uint32_t len);
};

} // namespace PX4_Mpu6000