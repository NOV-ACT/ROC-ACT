cmake_minimum_required(VERSION 3.16)

# 1. Bağımsız CMake projesi olan mreq alt modülü eklenir
add_subdirectory(mreq)
# 2. ESP-IDF build sistemi başlatılır
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(novact_fc)

# 3. ESP-IDF'e özel component dizinleri tanımlanır
set(EXTRA_COMPONENT_DIRS components)