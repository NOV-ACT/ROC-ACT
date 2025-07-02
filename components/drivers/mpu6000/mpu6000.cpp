#include "mpu6000.hpp"
#include "hal/Util.hpp" // For delay_micros
#include "hal/HAL.hpp" // For get_hal()

namespace PX4_Mpu6000 {

// MPU6000 registers
static constexpr uint8_t MPU6000_REG_SMPLRT_DIV     = 0x19;
static constexpr uint8_t MPU6000_REG_CONFIG         = 0x1A;
static constexpr uint8_t MPU6000_REG_GYRO_CONFIG    = 0x1B;
static constexpr uint8_t MPU6000_REG_ACCEL_CONFIG   = 0x1C;
static constexpr uint8_t MPU6000_REG_FIFO_EN        = 0x23;
static constexpr uint8_t MPU6000_REG_INT_PIN_CFG    = 0x37;
static constexpr uint8_t MPU6000_REG_INT_ENABLE     = 0x38;
static constexpr uint8_t MPU6000_REG_ACCEL_XOUT_H   = 0x3B;
static constexpr uint8_t MPU6000_REG_TEMP_OUT_H     = 0x41;
static constexpr uint8_t MPU6000_REG_GYRO_XOUT_H    = 0x43;
static constexpr uint8_t MPU6000_REG_PWR_MGMT_1     = 0x6B;
static constexpr uint8_t MPU6000_REG_WHO_AM_I       = 0x75;

// MPU6000 constants
static constexpr uint8_t MPU6000_WHO_AM_I_CONST     = 0x68; // Expected value for WHO_AM_I register

MPU6000::MPU6000(AP_HAL::SPI* spi_interface, uint8_t cs_pin) :
    _interface_type(Interface::SPI),
    _spi(spi_interface),
    _i2c(nullptr),
    _cs_pin(cs_pin),
    _address(0)
{
}

MPU6000::MPU6000(AP_HAL::I2C* i2c_interface, uint8_t address) :
    _interface_type(Interface::I2C),
    _spi(nullptr),
    _i2c(i2c_interface),
    _cs_pin(0),
    _address(address)
{
}

uint8_t MPU6000::read_reg(uint8_t reg) {
    uint8_t val = 0;
    if (_interface_type == Interface::SPI) {
        _spi->beginTransaction(_cs_pin, 1000000, 0); // 1MHz, mode 0
        _spi->transfer(reg | 0x80); // Set MSB for read
        val = _spi->transfer(0x00);
        _spi->endTransaction(_cs_pin);
    } else { // I2C
        val = _i2c->readRegister(_address, reg);
    }
    return val;
}

void MPU6000::write_reg(uint8_t reg, uint8_t value) {
    if (_interface_type == Interface::SPI) {
        _spi->beginTransaction(_cs_pin, 1000000, 0);
        _spi->transfer(reg & 0x7F); // Clear MSB for write
        _spi->transfer(value);
        _spi->endTransaction(_cs_pin);
    } else { // I2C
        _i2c->writeRegister(_address, reg, value);
    }
}

bool MPU6000::read_registers(uint8_t reg, uint8_t *data, uint32_t len) {
    if (_interface_type == Interface::SPI) {
        _spi->beginTransaction(_cs_pin, 1000000, 0);
        _spi->transfer(reg | 0x80); // Set MSB for read
        _spi->transfer(nullptr, data, len);
        _spi->endTransaction(_cs_pin);
    } else { // I2C
        return _i2c->read(_address, reg, data, len) == 0; // Assuming 0 means success
    }
    return true;
}

bool MPU6000::probe() {
    uint8_t who_am_i = read_reg(MPU6000_REG_WHO_AM_I);
    return who_am_i == MPU6000_WHO_AM_I_CONST;
}

bool MPU6000::init() {
    if (!probe()) {
        return false;
    }

    // Reset device
    write_reg(MPU6000_REG_PWR_MGMT_1, 0x80);
    AP_HAL::get_hal().util->delay_micros(10000); // 10ms delay

    // Wake up device
    write_reg(MPU6000_REG_PWR_MGMT_1, 0x00);
    AP_HAL::get_hal().util->delay_micros(1000); // 1ms delay

    // Configure sample rate (1kHz / (1 + SMPLRT_DIV))
    write_reg(MPU6000_REG_SMPLRT_DIV, 0x00); // 1kHz sample rate

    // Configure DLPF (Digital Low Pass Filter)
    write_reg(MPU6000_REG_CONFIG, 0x01); // DLPF_CFG = 1 (188Hz BW for gyro, 194Hz BW for accel)

    // Configure Gyro Full Scale Range
    write_reg(MPU6000_REG_GYRO_CONFIG, 0x00); // +/- 250 deg/s

    // Configure Accel Full Scale Range
    write_reg(MPU6000_REG_ACCEL_CONFIG, 0x00); // +/- 2g

    // Disable FIFO
    write_reg(MPU6000_REG_FIFO_EN, 0x00);

    // Disable interrupts
    write_reg(MPU6000_REG_INT_ENABLE, 0x00);

    return true;
}

bool MPU6000::read_accel_gyro(float& accel_x, float& accel_y, float& accel_z,
                             float& gyro_x, float& gyro_y, float& gyro_z) {
    uint8_t buffer[14]; // Accel (6 bytes), Temp (2 bytes), Gyro (6 bytes)

    if (!read_registers(MPU6000_REG_ACCEL_XOUT_H, buffer, sizeof(buffer))) {
        return false;
    }

    // Raw sensor data (16-bit signed integers)
    int16_t raw_accel_x = (int16_t)((buffer[0] << 8) | buffer[1]);
    int16_t raw_accel_y = (int16_t)((buffer[2] << 8) | buffer[3]);
    int16_t raw_accel_z = (int16_t)((buffer[4] << 8) | buffer[5]);

    int16_t raw_gyro_x = (int16_t)((buffer[8] << 8) | buffer[9]);
    int16_t raw_gyro_y = (int16_t)((buffer[10] << 8) | buffer[11]);
    int16_t raw_gyro_z = (int16_t)((buffer[12] << 8) | buffer[13]);

    // Convert raw values to engineering units (example scaling for +/- 2g and +/- 250 deg/s)
    // These scaling factors depend on the ACCEL_CONFIG and GYRO_CONFIG settings
    // For +/- 2g, sensitivity is 16384 LSB/g
    // For +/- 250 deg/s, sensitivity is 131 LSB/(deg/s)
    accel_x = static_cast<float>(raw_accel_x) / 16384.0f;
    accel_y = static_cast<float>(raw_accel_y) / 16384.0f;
    accel_z = static_cast<float>(raw_accel_z) / 16384.0f;

    gyro_x = static_cast<float>(raw_gyro_x) / 131.0f;
    gyro_y = static_cast<float>(raw_gyro_y) / 131.0f;
    gyro_z = static_cast<float>(raw_gyro_z) / 131.0f;

    return true;
}

} // namespace PX4_Mpu6000