cmake_minimum_required(VERSION 3.28)

project(xgl2d)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

include_directories(include)

set(LIB_SOURCES src/mesh/xmesh.cpp src/mesh/xquadmesh.cpp)
set(TEST_SOURCES src/main.cpp ${LIB_SOURCES})

add_library(libxgl2d STATIC ${LIB_SOURCES})
add_library(xgl2d SHARED ${LIB_SOURCES})
add_executable(testxgl2d ${TEST_SOURCES})

target_link_libraries(libxgl2d glfw GLEW::GLEW OpenGL::GL glm::glm)
target_link_libraries(xgl2d glfw GLEW::GLEW OpenGL::GL glm::glm)
target_link_libraries(testxgl2d glfw GLEW::GLEW OpenGL::GL glm::glm)
