project(player_output)

MESSAGE(STATUS "PLAYER BINARY dir: " ${${PROJECT_NAME}_BINARY_DIR})
MESSAGE(STATUS "PLAYER SOURCE dir: " ${${PROJECT_NAME}_SOURCE_DIR})

aux_source_directory(${PROJECT_SOURCE_DIR} PLAYER_FILES)

message(STATUS "files: " ${PLAYER_FILES})

add_library(${PROJECT_NAME} STATIC ${PLAYER_FILES})

include_directories(${PROJECT_SOURCE_DIR}
)

link_directories()

target_link_libraries(${PROJECT_NAME}
)