cmake_minimum_required(VERSION 3.28)

project(xgl2d)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

include_directories(include)

add_executable(testxgl2d src/main.cpp)

target_link_libraries(testxgl2d glfw GLEW::GLEW OpenGL::GL glm::glm)
