project(logger_output)

MESSAGE(STATUS "LOGGER BINARY dir: " ${${PROJECT_NAME}_BINARY_DIR})
MESSAGE(STATUS "LOGGER SOURCE dir: " ${${PROJECT_NAME}_SOURCE_DIR})

aux_source_directory(${PROJECT_SOURCE_DIR} LOGGER_FILES)

message(STATUS "files: " ${LOGGER_FILES})

add_library(${PROJECT_NAME} STATIC ${LOGGER_FILES})

include_directories(${PROJECT_SOURCE_DIR}
)

link_directories()

target_link_libraries(${PROJECT_NAME}
)