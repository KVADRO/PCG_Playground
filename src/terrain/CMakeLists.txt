cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME terrain)
project(${TARGET_NAME})

set (CMAKE_CXX_STANDARD 17)

set(EXTERNAL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

set(SOURCES)
set(LIBRARIES glfw glad)
set(INCLUDES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src 
    ${EXTERNAL_LIB_PATH}/glfw/include
    ${EXTERNAL_LIB_PATH}/glm
    ${EXTERNAL_LIB_PATH}/stb
    ${EXTERNAL_LIB_PATH}/glad/include)
set(COMPILE_DEFINITIONS GLFW_INCLUDE_NONE)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink 
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_BINARY_DIR}/data)

list(APPEND SOURCES
    src/main.cpp
    src/GLUtils.h
    src/CallOnDestroy.h
    src/FrameCounter.h
    src/Game.h
    src/Game.cpp
    src/Input.h
    src/Input.cpp
    src/stb_impl.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDES})

target_compile_definitions(${TARGET_NAME} PUBLIC ${COMPILE_DEFINITIONS})

# GLFW

add_subdirectory(${EXTERNAL_LIB_PATH}/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)

# GLAD

add_subdirectory(${EXTERNAL_LIB_PATH}/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)

# GLM

# STB

# Link libs

target_link_libraries(${TARGET_NAME} PUBLIC ${LIBRARIES})