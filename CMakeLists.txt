cmake_minimum_required(VERSION 3.22)

project(gobang)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
# /usr/lib/x86_64-linux-gnu/cmake/sdl2_image/sdl2_image-config.cmake
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_subdirectory(
    third_party/spdlog
)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)
# set(SOURCES_FILES
#     src/main.cpp
#     src/logger/logger.h
#     src/logger/logger.cpp
# )

file(GLOB_RECURSE SOURCES_FILES 
    src/main.cpp 
    src/logger/*
)

add_executable(${PROJECT_NAME} ${SOURCES_FILES})

target_link_libraries(${TARGET_PROJECT} LINK_PUBLIC ${TARGET_LOGGER})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
# add_custom_command(
#     TARGET ${PROJECT_NAME}
#     POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/*.png" ${PROJECT_BINARY_DIR}
# )

