# Main application component
set(COMPONENT_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tasks/imu_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tasks/baro_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tasks/state_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tasks/pyro_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tasks/logger_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tasks/telemetry_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../freertos/startup.c
)

set(COMPONENT_PRIV_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../msg
    ${CMAKE_CURRENT_SOURCE_DIR}/../mreq/external/nanopb
    ${CMAKE_BINARY_DIR}/gen
    ${CMAKE_BINARY_DIR}/autogen
)

idf_component_register(SRCS ${COMPONENT_SRCS}
                       GENERATED_SRCS ${ALL_PROJECT_GENERATED_SRCS}
                       INCLUDE_DIRS ${COMPONENT_PRIV_INCLUDE_DIRS}
                       REQUIRES mreq freertos)
message(STATUS "main/CMakeLists.txt - ALL_PROJECT_GENERATED_SRCS: ${ALL_PROJECT_GENERATED_SRCS}")