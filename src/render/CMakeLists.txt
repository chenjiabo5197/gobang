cmake_minimum_required(VERSION 3.0)
project(render_output)

MESSAGE(STATUS "RENDER BINARY dir: " ${${PROJECT_NAME}_BINARY_DIR})
MESSAGE(STATUS "RENDER SOURCE dir: " ${${PROJECT_NAME}_SOURCE_DIR})

aux_source_directory(${PROJECT_SOURCE_DIR} RENDER_FILES)

message(STATUS "files: " ${RENDER_FILES})

add_library(${PROJECT_NAME} STATIC ${RENDER_FILES})

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIRS} 
    ${SDL2_IMAGE_INCLUDE_DIRS} 
    ${SDL2_TTF_INCLUDE_DIRS} 
    ${SDL2_MIXER_INCLUDE_DIRS}
    ../logger
    ../utils
    ../common
)

# link_directories()

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${SDL2_TTF_LIBRARIES} 
    ${SDL2_MIXER_LIBRARIES}
)