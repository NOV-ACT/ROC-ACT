set(PROTO_FILES
    ${COMPONENT_DIR}/event.proto
    ${COMPONENT_DIR}/fused_sensor_data.proto
    ${COMPONENT_DIR}/flight_state.proto
    ${COMPONENT_DIR}/pyro_command.proto
    ${COMPONENT_DIR}/sensor_baro.proto
    ${COMPONENT_DIR}/sensor_imu.proto
)

# Modified GenerateProtobuf.cmake'i include et
include(${COMPONENT_DIR}/../../mreq/cmake/GenerateProtobuf.cmake)

# Configure-time'da protobuf dosyalarını generate et
generate_protobuf_configure_time("${PROTO_FILES}")


# Topic registry generate et (eğer varsa)
include(${COMPONENT_DIR}/../../mreq/cmake/GenerateTopicRegistry.cmake)
# generate_topic_registry(${PROTO_FILES} ${COMPONENT_LIB})
generate_topic_registry_configure_time(${PROTO_FILES} ${CMAKE_BINARY_DIR}/autogen)

# Generate edilen dosyaları source olarak kullan
file(GLOB GENERATED_SRCS "${PROTOBUF_INCLUDE_DIR}/*.pb.c")

# Component'i kaydet
idf_component_register(
    SRCS ${GENERATED_SRCS}
    INCLUDE_DIRS "${CMAKE_BINARY_DIR}/autogen"
)

target_link_libraries(${COMPONENT_LIB} PUBLIC mreq)