cmake_minimum_required(VERSION 3.17)
project(NeonBoids)

set(CMAKE_CXX_STANDARD 17)

add_executable(NeonBoids main.cpp)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glew REQUIRED)

target_link_libraries(NeonBoids OpenGL glfw glew)
