project(chess_output)

MESSAGE(STATUS "CHESS BINARY dir: " ${${PROJECT_NAME}_BINARY_DIR})
MESSAGE(STATUS "CHESS SOURCE dir: " ${${PROJECT_NAME}_SOURCE_DIR})

aux_source_directory(${PROJECT_SOURCE_DIR} CHESS_FILES)

message(STATUS "files: " ${CHESS_FILES})

add_library(${PROJECT_NAME} STATIC ${CHESS_FILES})

include_directories(${PROJECT_SOURCE_DIR}
)

link_directories()

target_link_libraries(${PROJECT_NAME}
    render_output
)
