# Main application component

idf_component_register(
    SRCS "main.cpp" ${PROJECT_GENERATED_PB_SRCS} ${PROJECT_GENERATED_TOPIC_REGISTRY_SRCS}
    REQUIRES drivers core hal tasks freertos_config msg_proto
)

target_link_libraries(${COMPONENT_LIB} PRIVATE mreq)