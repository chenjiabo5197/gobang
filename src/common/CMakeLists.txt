cmake_minimum_required(VERSION 3.0)
project(common_output)

MESSAGE(STATUS "COMMON BINARY dir: " ${${PROJECT_NAME}_BINARY_DIR})
MESSAGE(STATUS "COMMON SOURCE dir: " ${${PROJECT_NAME}_SOURCE_DIR})

aux_source_directory(${PROJECT_SOURCE_DIR} COMMON_FILES)

message(STATUS "files: " ${COMMON_FILES})

add_library(${PROJECT_NAME} STATIC ${COMMON_FILES})

include_directories(
    ${PROJECT_SOURCE_DIR}
    ../render
    ../logger
    ../utils
)

link_directories(
    ../render/build
)

target_link_libraries(${PROJECT_NAME})
