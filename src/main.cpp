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

int main() {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Cherno used 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // Cherno used 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // or COMPAT

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


//    GLEW INIT
    if (glewInit() != GLEW_OK)
        std::cout << "GLEW ERROR!" << std::endl;

//    print OpenGL version
    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
                -50.0f, -50.0f, 0.0f, 0.0f,    // 0
                50.0f, -50.0f, 1.0f, 0.0f,    // 1
                50.0f, 50.0f, 1.0f, 1.0f,    // 2
                -50.0f, 50.0f, 0.0f, 1.0f     // 3
        };

        unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        // Turn on blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // VertexArray instance
        VertexArray va;
        // Vertex Buffer
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Index buffer
        IndexBuffer ib(indices, 6);

        // GLM matrix
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);

        // Create shader
        Shader shader("res/shaders/Basic.shader");

        shader.Bind();
        // Set uniform for shader
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        // Load Texture
        Texture texture("res/textures/bas.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
//        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        // Unbind all
        shader.Unbind();
        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        // ImGUI
        // Create context and initalize
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        // Set dark theme
        ImGui::StyleColorsDark();

        // Color channel variables for animating the rectangle
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            // Clear the screen
            renderer.Clear();

            // Start new imgui frame
            ImGui_ImplGlfwGL3_NewFrame();

            // bind shaders
            shader.Bind();

            // interactive Model Matrix
            // 1st Draw
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                // Draw 1st time
                renderer.Draw(va, ib, shader);
            }
            // 2nd Draw
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                // Draw 1st time
                renderer.Draw(va, ib, shader);
            }

            // Change the red channel and check values
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            // Imgui window
            ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, 960.0f);
            ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, 960.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,ImGui::GetIO().Framerate);

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