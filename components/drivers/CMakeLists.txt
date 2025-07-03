idf_component_register(SRCS 
                        "include/drivers/ImuDriver.cpp"
                        "include/drivers/BarometerDriver.cpp" 
                        "include/drivers/PyroDriver.cpp" 
                        "include/drivers/SDCardDriver.cpp" 
                        "include/drivers/TelemetryDriver.cpp" 
                        "include/drivers/mpu6000/mpu6000.cpp"
                       INCLUDE_DIRS "include" "include/drivers/mpu6000"
                       PRIV_REQUIRES novact_hal)