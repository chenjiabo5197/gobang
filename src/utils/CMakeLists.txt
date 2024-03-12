project(utils_output)

MESSAGE(STATUS "UTILS BINARY dir: " ${${PROJECT_NAME}_BINARY_DIR})
MESSAGE(STATUS "UTILS SOURCE dir: " ${${PROJECT_NAME}_SOURCE_DIR})

aux_source_directory(${PROJECT_SOURCE_DIR} UTILS_FILES)

message(STATUS "files: " ${UTILS_FILES})

add_library(${PROJECT_NAME} STATIC ${UTILS_FILES})

include_directories(${PROJECT_SOURCE_DIR}
)

link_directories()

target_link_libraries(${PROJECT_NAME}
)