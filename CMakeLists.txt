cmake_minimum_required(VERSION 3.28)

project(xgl2d)
set(CMAKE_CXX_STANDARD 23)
# 设置默认构建类型为 Debug
set(CMAKE_BUILD_TYPE
    Debug
    CACHE STRING "Build type" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(Freetype REQUIRED)
find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

# 手动指定 FreeType 的路径
set(FREETYPE_INCLUDE_DIRS "/opt/homebrew/include/freetype2")
set(FREETYPE_LIBRARIES "/opt/homebrew/lib/libfreetype.dylib")

include_directories(include ${FREETYPE_INCLUDE_DIRS})
include_directories(src)

set(LIB_SOURCES
    src/mesh/text/text.cpp src/logger/logger.cpp src/texture/texturepool.cpp
    src/texture/MaxRectsBinPack.cpp src/shader/shader.cpp src/mesh/mesh.cpp)

set(TEST_SOURCES src/main.cpp)

add_library(libxgl2d STATIC ${LIB_SOURCES})
add_library(xgl2d SHARED ${LIB_SOURCES})
add_executable(testxgl2d ${TEST_SOURCES} ${LIB_SOURCES})

target_link_libraries(
  libxgl2d
  ${FREETYPE_LIBRARY}
  spdlog::spdlog
  glfw
  GLEW::GLEW
  OpenGL::GL
  glm::glm)
target_link_libraries(
  xgl2d
  ${FREETYPE_LIBRARY}
  spdlog::spdlog
  glfw
  GLEW::GLEW
  OpenGL::GL
  glm::glm)
target_link_libraries(
  testxgl2d
  ${FREETYPE_LIBRARY}
  spdlog::spdlog
  glfw
  GLEW::GLEW
  OpenGL::GL
  glm::glm)
