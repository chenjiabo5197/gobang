project(manage_output)

MESSAGE(STATUS "MANAGE BINARY dir: " ${${PROJECT_NAME}_BINARY_DIR})
MESSAGE(STATUS "MANAGE SOURCE dir: " ${${PROJECT_NAME}_SOURCE_DIR})

aux_source_directory(${PROJECT_SOURCE_DIR} MANAGE_FILES)

message(STATUS "files: " ${MANAGE_FILES})

add_library(${PROJECT_NAME} STATIC ${MANAGE_FILES})

include_directories(${PROJECT_SOURCE_DIR}
)

link_directories()

target_link_libraries(${PROJECT_NAME}
    chess_output
    common_output
)