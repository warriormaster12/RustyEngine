cmake_minimum_required(VERSION 3.16.0)
project(RustyEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRC
${PROJECT_SOURCE_DIR}/src/renderer/*
${PROJECT_SOURCE_DIR}/src/main/*
${PROJECT_SOURCE_DIR}/src/window/window.cpp
${PROJECT_SOURCE_DIR}/src/scene/*
${PROJECT_SOURCE_DIR}/src/logger/logger.cpp
)

add_executable(RustyEngine ${SRC})

if (UNIX AND NOT APPLE)
    set (LINUX TRUE)
    if(DGLFW_USE_WAYLAND=ON)
        set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WAYLAND_KHR)
    else()
        set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_XLIB_KHR)
    endif ()
endif ()


target_include_directories(RustyEngine PUBLIC
    src/main/
    src/renderer/
    src/window/
    src/scene/
    src/logger/
    lib/glm/
)


add_subdirectory(src/window/third-party/glfw/)
target_link_libraries(RustyEngine PRIVATE glfw)
add_subdirectory(src/logger/third-party/spdlog/)
target_link_libraries(RustyEngine PRIVATE spdlog)

file(COPY ${PROJECT_SOURCE_DIR}/src/shaders DESTINATION ${PROJECT_BINARY_DIR})



 
