#define GLEW_STATIC

#include <csignal>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"

int main() {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Neon Boids", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW ERROR!" << std::endl;

    // print OpenGL version
    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        // Turn on blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Index buffer
        Renderer renderer;

        // Create ImGui context and initialize
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);

        // Set dark theme
        ImGui::StyleColorsDark();

        test::TestClearColor test;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            // Clear the screen
            renderer.Clear();

            test.OnUpdate(0.0f);
            test.OnRender();

            // Start new imgui frame
            ImGui_ImplGlfwGL3_NewFrame();

            test.OnImGuiRender();

            // Render ImGui
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    // Cleanup ImGui
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    // Terminate glfw
    glfwTerminate();
    return 0;
}