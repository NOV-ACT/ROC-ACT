# Main application component

idf_component_register(
    SRCS "main.cpp" ${PROJECT_GENERATED_PB_SRCS} ${PROJECT_GENERATED_TOPIC_REGISTRY_SRCS}
    REQUIRES drivers novact_core novact_hal tasks msg_proto
)
