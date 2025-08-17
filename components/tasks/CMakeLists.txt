

# Task kaynak dosyalarını topla
set(TASK_SOURCES
    "imu_task.cpp"
    "baro_task.cpp"
    "state_task.cpp"
    "pyro_task.cpp"
    "logger_task.cpp"
    "telemetry_task.cpp"
    "led_status_task.cpp"
)

# Debug için kaynak dosyalarını listele
message(STATUS "Task Sources: ${TASK_SOURCES}")

# Component'i kaydet
idf_component_register(
    SRCS ${TASK_SOURCES}
    INCLUDE_DIRS
        "."
        "include"
    REQUIRES
        drivers
        novact_comm
        novact_core
        msg_proto
        # C++ için gerekli ESP-IDF bileşenleri
        cxx
        pthread
)