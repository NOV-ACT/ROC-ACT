idf_component_register(SRCS 
                        "include/core/StateManager.cpp" 
                        "include/core/Logger.cpp" 
                        "include/core/SensorFusion.cpp" 
                        "include/core/MessagingClient.cpp"
                       INCLUDE_DIRS "include" "${CMAKE_BINARY_DIR}/autogen"
                       REQUIRES msg_proto drivers)
