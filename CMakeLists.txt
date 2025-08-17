cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(novact_fc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(MREQ_PLATFORM_FREERTOS)

# 1. Bağımsız CMake projesi olan mreq alt modülü eklenir
add_subdirectory(components/msg_proto/mreq)

# 3. ESP-IDF'e özel component dizinleri tanımlanır
set(EXTRA_COMPONENT_DIRS components)