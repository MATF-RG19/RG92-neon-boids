cmake_minimum_required(VERSION 3.17)
project(NeonBoids)

set(CMAKE_CXX_STANDARD 17)

add_executable(NeonBoids src/main.cpp src/Renderer.h src/Renderer.cpp src/VertexBuffer.h src/VertexBuffer.cpp src/IndexBuffer.h src/IndexBuffer.cpp src/VertexArray.h src/VertexArray.cpp src/VertexBufferLayout.h src/VertexBufferLayout.cpp src/Shader.h src/Shader.cpp)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(NeonBoids OpenGL glfw GLEW)
