# Modified GenerateProtobuf.cmake'i include et
include(${COMPONENT_DIR}/mreq/cmake/GenerateProtobuf.cmake)
include(${COMPONENT_DIR}/mreq/cmake/GenerateTopicRegistry.cmake)

# Proto dosyalarını tanımla
set(PROTO_FILES
    ${COMPONENT_DIR}/proto/event.proto
    ${COMPONENT_DIR}/proto/fused_sensor_data.proto
    ${COMPONENT_DIR}/proto/flight_state.proto
    ${COMPONENT_DIR}/proto/pyro_command.proto
    ${COMPONENT_DIR}/proto/sensor_baro.proto
    ${COMPONENT_DIR}/proto/sensor_imu.proto
)

# mreq'in cmake dosyalarını include et
include(${COMPONENT_DIR}/mreq/cmake/GenerateProtobuf.cmake)
include(${COMPONENT_DIR}/mreq/cmake/GenerateTopicRegistry.cmake)

# Otomatik generate edilecek dosyalar için dizin
set(AUTOGEN_DIR "${CMAKE_BINARY_DIR}/autogen")
file(MAKE_DIRECTORY ${AUTOGEN_DIR})

# Configure-time'da protobuf dosyalarını generate et
generate_protobuf_configure_time("${PROTO_FILES}")

# Topic registry generate et
generate_topic_registry_configure_time("${PROTO_FILES}" "${AUTOGEN_DIR}")

# Generate edilen .pb.c dosyalarını bul
file(GLOB GENERATED_PB_SRCS "${AUTOGEN_DIR}/*.pb.c")

# Generate edilen topic registry dosyalarını bul (varsa)
file(GLOB GENERATED_REGISTRY_SRCS "${AUTOGEN_DIR}/topic_registry*.c*")

# Tüm generate edilen source dosyalarını birleştir
set(ALL_GENERATED_SRCS ${GENERATED_PB_SRCS} ${GENERATED_REGISTRY_SRCS})

# Eğer kendi source dosyalarınız varsa onları da ekleyin
set(COMPONENT_SRCS
    # Kendi .cpp/.c dosyalarınızı buraya ekleyin
    # main.cpp gibi
    ${ALL_GENERATED_SRCS}
)

# Component'i register et
idf_component_register(
    SRCS 
        ${COMPONENT_SRCS}
        ${COMPONENT_DIR}/mreq/external/nanopb/pb_common.c 
        ${COMPONENT_DIR}/mreq/external/nanopb/pb_decode.c 
        ${COMPONENT_DIR}/mreq/external/nanopb/pb_encode.c
    INCLUDE_DIRS 
        "${AUTOGEN_DIR}"
        "${COMPONENT_DIR}/mreq/include"
        "${COMPONENT_DIR}/mreq/external/nanopb"
    REQUIRES
        # ESP-IDF component dependencies buraya
        freertos
        # Diğer gerekli component'ler
)

# mreq kütüphanesini link et
target_link_libraries(${COMPONENT_LIB} PUBLIC mreq)

# Debug için generate edilen dosyaları göster
message(STATUS "Generated protobuf sources: ${GENERATED_PB_SRCS}")
message(STATUS "Generated registry sources: ${GENERATED_REGISTRY_SRCS}")
message(STATUS "Autogen directory: ${AUTOGEN_DIR}")

# Compiler flags (isteğe bağlı)
target_compile_options(${COMPONENT_LIB} PUBLIC
    -Wno-unused-parameter
    -Wno-missing-field-initializers
)

# C++ standard ayarı (gerekirse)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)