idf_component_register(SRCS "imu_task.cpp" "baro_task.cpp" "state_task.cpp" "pyro_task.cpp" "logger_task.cpp" "telemetry_task.cpp"
                       INCLUDE_DIRS "." "${CMAKE_BINARY_DIR}/autogen"
                       REQUIRES drivers core freertos_config msg_proto)