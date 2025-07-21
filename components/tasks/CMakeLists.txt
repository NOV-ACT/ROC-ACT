# tasks/CMakeLists.txt - İyileştirilmiş versiyon

# NOV-HAB kütüphanesi için değişkenler
set(NOV_HAB_SRC_DIR ${COMPONENT_DIR}/NOV-HAB)

# NOV-HAB kaynak dosyalarını topla
file(GLOB NOV_HAB_SOURCES 
    ${NOV_HAB_SRC_DIR}/src/*.cpp
)

# Task kaynak dosyalarını topla
set(TASK_SOURCES
    "imu_task.cpp"
    "baro_task.cpp" 
    "state_task.cpp"
    "pyro_task.cpp"
    "logger_task.cpp"
    "telemetry_task.cpp"
)

# Tüm kaynak dosyalarını birleştir
set(ALL_SOURCES ${NOV_HAB_SOURCES} ${TASK_SOURCES})

# Debug için kaynak dosyalarını listele
message(STATUS "NOV-HAB Sources: ${NOV_HAB_SOURCES}")
message(STATUS "Task Sources: ${TASK_SOURCES}")

# Component'i kaydet
idf_component_register(
    SRCS ${ALL_SOURCES}
    INCLUDE_DIRS 
        "${NOV_HAB_SRC_DIR}/include" 
        "." 
        "${CMAKE_BINARY_DIR}/autogen"
    REQUIRES 
        drivers 
        novact_core 
        msg_proto
        # C++ için gerekli ESP-IDF bileşenleri
        cxx
        pthread
)

# C++17 standardını zorla (NOV-HAB için gerekli olabilir)
target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_17)

# NOV-HAB için özel tanımlar (gerekirse)
target_compile_definitions(${COMPONENT_LIB} PRIVATE
    NOV_HAB_ESP32=1
    # Diğer gerekli tanımlar...
)

# Compiler flags (C++ için)
target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall
    -Wextra
    -std=c++17
)