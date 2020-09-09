cmake_minimum_required(VERSION 3.17)
project(NeonBoids)

set(CMAKE_CXX_STANDARD 17)
include_directories(src/vendor)

add_executable(NeonBoids src/main.cpp src/Renderer.h src/Renderer.cpp src/VertexBuffer.h src/VertexBuffer.cpp src/IndexBuffer.h src/IndexBuffer.cpp src/VertexArray.h src/VertexArray.cpp src/VertexBufferLayout.h src/VertexBufferLayout.cpp src/Shader.h src/Shader.cpp src/vendor/stb_image/stb_image.h src/vendor/stb_image/stb_image.cpp src/Texture.h src/Texture.cpp src/vendor/imgui/imgui_demo.cpp src/vendor/imgui/imgui.h src/vendor/imgui/imgui.cpp src/vendor/imgui/imconfig.h src/vendor/imgui/imgui_internal.h src/vendor/imgui/imgui_draw.cpp src/vendor/imgui/stb_rect_pack.h src/vendor/imgui/stb_textedit.h src/vendor/imgui/stb_truetype.h src/vendor/imgui/imgui_impl_glfw_gl3.h src/vendor/imgui/imgui_impl_glfw_gl3.cpp src/tests/TestClearColor.h src/tests/TestClearColor.cpp src/tests/Test.cpp src/tests/TestTexture2D.cpp src/tests/TestTexture2D.h)



find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(NeonBoids OpenGL glfw GLEW)
