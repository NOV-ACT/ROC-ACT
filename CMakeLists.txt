cmake_minimum_required(VERSION 3.16)

project(novact_fc LANGUAGES CXX)
# Include ESP-IDF project build system
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set FreeRTOS platform for MREQ
set(MREQ_USE_FREERTOS ON)

add_subdirectory(mreq)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mreq/cmake)

include(GenerateProtobuf)
include(GenerateTopicRegistry)

# Define protobuf files
set(MREQ_PROTO_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/msg/event.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/msg/flight_state.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/msg/pyro_command.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/msg/sensor_baro.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/msg/sensor_imu.proto
)

add_executable(novact_fc src/main.cpp)

# Generate protobuf files and get the list of generated sources
set(PROJECT_GENERATED_PB_SRCS "") # Initialize as empty list
generate_protobuf("${MREQ_PROTO_FILES}" novact_fc)
message(STATUS "Root - PROJECT_GENERATED_PB_SRCS: ${PROJECT_GENERATED_PB_SRCS}")

# Generate topic registry files
generate_topic_registry(${MREQ_PROTO_FILES})
set(PROJECT_GENERATED_TOPIC_REGISTRY_SRCS
    ${CMAKE_BINARY_DIR}/autogen/topic_registry_autogen.cpp
)
message(STATUS "Root - PROJECT_GENERATED_TOPIC_REGISTRY_SRCS: ${PROJECT_GENERATED_TOPIC_REGISTRY_SRCS}")

# Combine all generated sources into a single list
set(ALL_PROJECT_GENERATED_SRCS ${PROJECT_GENERATED_PB_SRCS} ${PROJECT_GENERATED_TOPIC_REGISTRY_SRCS})
message(STATUS "Root - ALL_PROJECT_GENERATED_SRCS: ${ALL_PROJECT_GENERATED_SRCS}")


# The 'main' component is automatically found by ESP-IDF if it's in the 'main' subdirectory.
# No need for add_subdirectory(main) here.