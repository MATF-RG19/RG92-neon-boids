#define GLEW_STATIC

#include <csignal>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

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

        test::Test *currentTest;
        auto *testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture");


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            // Clear the screen
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            renderer.Clear();

            // Start new imgui frame
            ImGui_ImplGlfwGL3_NewFrame();

            if (currentTest) {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("-<")) {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();

                ImGui::End();
            }

            // Render ImGui
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }

        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }
    // Cleanup ImGui
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    // Terminate glfw
    glfwTerminate();
    return 0;
}