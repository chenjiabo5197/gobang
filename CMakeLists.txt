cmake_minimum_required(VERSION 3.22)

project(gobang)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(spdlog REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)
# set(SOURCES_FILES
#     src/main.cpp
#     src/logger/logger.h
#     src/logger/logger.cpp
# )

file(GLOB_RECURSE SOURCES_FILES 
    src/main.cpp
    src/common.h
    src/logger/*
    src/utils/*
    src/render/sdl_*
    src/chess/*
    src/manage/*
    src/player/*
    # third_party/spdlog/*
)

add_executable(${PROJECT_NAME} ${SOURCES_FILES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
# target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/config" ${PROJECT_BINARY_DIR}/config
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources" ${PROJECT_BINARY_DIR}/resources
)

