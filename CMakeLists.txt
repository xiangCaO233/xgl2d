cmake_minimum_required(VERSION 3.28)

project(xgl2d)
set(CMAKE_CXX_STANDARD 17)
# 设置默认构建类型为 Debug
set(CMAKE_BUILD_TYPE
    Debug
    CACHE STRING "Build type" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

include_directories(include)

set(LIB_SOURCES
    src/mesh/shape/Oval.cpp
    src/mesh/shape/Shape.cpp
    src/mesh/shape/Quad.cpp
    src/mesh/xmesh.cpp
    src/mesh/shape/xquadmesh.cpp
    src/shader/shader.cpp
    src/texture/texture.cpp)
set(TEST_SOURCES src/main.cpp ${LIB_SOURCES})

add_library(libxgl2d STATIC ${LIB_SOURCES})
add_library(xgl2d SHARED ${LIB_SOURCES})
add_executable(testxgl2d ${TEST_SOURCES} ${LIB_SOURCES})

target_link_libraries(libxgl2d glfw GLEW::GLEW OpenGL::GL glm::glm)
target_link_libraries(xgl2d glfw GLEW::GLEW OpenGL::GL glm::glm)
target_link_libraries(testxgl2d glfw GLEW::GLEW OpenGL::GL glm::glm)
