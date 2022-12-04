cmake_minimum_required(VERSION 3.16.0)
project(RustyEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_SHARED_LIBS OFF)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB_RECURSE SRC
${PROJECT_SOURCE_DIR}/src/renderer/*
${PROJECT_SOURCE_DIR}/src/main/*
${PROJECT_SOURCE_DIR}/src/window/window.cpp
${PROJECT_SOURCE_DIR}/src/scene/*
${PROJECT_SOURCE_DIR}/src/logger/logger.cpp
${PROJECT_SOURCE_DIR}/src/tools/model_loading/third-party/tiny_gltf.cpp
${PROJECT_SOURCE_DIR}/src/tools/model_loading/model_loader.cpp
)

add_executable(RustyEngine ${SRC})


if (UNIX AND NOT APPLE)
    set (LINUX TRUE)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_XLIB_KHR)
endif ()


target_include_directories(RustyEngine PUBLIC
    src/main/
    src/renderer/
    src/window/
    src/scene/
    src/logger/
    src/logger/third-party/
    src/tools/model_loading/third-party/
    src/tools/model_loading/
    lib/glm/
)


add_subdirectory(src/window/third-party/glfw/)
target_link_libraries(RustyEngine PRIVATE glfw)

file(COPY ${PROJECT_SOURCE_DIR}/src/shaders DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/samples DESTINATION ${PROJECT_BINARY_DIR})



 
