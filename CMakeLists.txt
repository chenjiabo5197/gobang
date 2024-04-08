cmake_minimum_required(VERSION 3.0)
project(gobang)

# 如果代码需要支持C++11，就直接加上这句
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
# 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
add_definitions("-Wall -g")

MESSAGE(STATUS "MAIN BINARY dir: " ${${PROJECT_NAME}_BINARY_DIR})
MESSAGE(STATUS "MAIN SOURCE dir: " ${${PROJECT_NAME}_SOURCE_DIR})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(spdlog REQUIRED)

# 设置变量，下面的代码都可以用到
# set(GOOGLE_PROTOBUF_DIR ${PROJECT_SOURCE_DIR}/protobuf)
# set(PROTO_PB_DIR ${PROJECT_SOURCE_DIR}/proto_pb2)
# set(PROTO_BUF_DIR ${PROJECT_SOURCE_DIR}/proto_buf)

# 编译子文件夹的CMakeLists.txt
add_subdirectory(src/chess)
add_subdirectory(src/common)
add_subdirectory(src/logger)
add_subdirectory(src/manage)
add_subdirectory(src/player)
add_subdirectory(src/render)
add_subdirectory(src/utils)

# 规定.h头文件路径
include_directories(${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/chess
    ${PROJECT_SOURCE_DIR}/src//common
    ${PROJECT_SOURCE_DIR}/src//logger
    ${PROJECT_SOURCE_DIR}/src//manage
    ${PROJECT_SOURCE_DIR}/src//player
    ${PROJECT_SOURCE_DIR}/src//render
    ${PROJECT_SOURCE_DIR}/src//utils
)

# 生成可执行文件
add_executable(${PROJECT_NAME} src/main.cpp )

# 链接操作
target_link_libraries(${PROJECT_NAME}
    chess_output
    common_output
    logger_output
    manage_output
    player_output
    render_output
    utils_output
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/config" ${PROJECT_BINARY_DIR}/config
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources" ${PROJECT_BINARY_DIR}/resources
)

MESSAGE(STATUS "copy config dir to" ${PROJECT_BINARY_DIR}/config)
MESSAGE(STATUS "copy resources dir to" ${PROJECT_BINARY_DIR}/resources)
