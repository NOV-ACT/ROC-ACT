cmake_minimum_required(VERSION 3.16)

# Include ESP-IDF project build system
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(novact_fc)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set FreeRTOS platform for MREQ
set(MREQ_USE_FREERTOS ON)
 if(NOT CMAKE_BUILD_EARLY_EXPANSION) 
     set_source_files_properties("esp32s2beta/rtc_clk.c" PROPERTIES 
                                 COMPILE_FLAGS "-fno-jump-tables -fno-tree-switch-conversion") 
 endif() 

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mreq/cmake)
include(GenerateProtobuf)
include(GenerateTopicRegistry)

# Define protobuf files (relative to project root)
set(MREQ_PROTO_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../components/msg_proto/event.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/../components/msg_proto/flight_state.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/../components/msg_proto/pyro_command.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/../components/msg_proto/sensor_baro.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/../components/msg_proto/sensor_imu.proto
)

generate_protobuf("${MREQ_PROTO_FILES}" PROJECT_GENERATED_PB_SRCS)
generate_topic_registry(${MREQ_PROTO_FILES} PROJECT_GENERATED_TOPIC_REGISTRY_SRCS)

# Protobuf dosyaları derleme kaynakları olarak eklenir
set(PROJECT_GENERATED_PB_SRCS ${PROJECT_GENERATED_PB_SRCS} PARENT_SCOPE)
set(PROJECT_GENERATED_TOPIC_REGISTRY_SRCS ${PROJECT_GENERATED_TOPIC_REGISTRY_SRCS} PARENT_SCOPE)


set(EXTRA_COMPONENT_DIRS components mreq)