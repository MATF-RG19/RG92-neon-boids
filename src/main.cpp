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
#include "fstream"

int main() {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Cherno used 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // Cherno used 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // or COMPAT

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
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
                -0.5f, -0.5f, 0.0f, 0.0f, // 0
                0.5f, -0.5f, 1.0f, 0.0f, // 1
                0.5f, 0.5f, 1.0f, 1.0f, // 2
                -0.5f, 0.5f, 0.0f, 1.0f // 3
        };

        unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        // Turn on blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Vertex array object
        unsigned int vao;


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

        // Create shader
        Shader shader("res/shaders/Basic.shader");

        shader.Bind();
        // Set uniform for shader
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        // Load Texture
        Texture texture("res/textures/bas.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        // Unbind all
        shader.Unbind();
        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        // Color channel variables for animating the rectangle
        float r = 0.0f;
        float increment = 0.05f;


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            // Clear the screen
            renderer.Clear();
            // bind shaders
            shader.Bind();
            // Set Rectangle color
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            // Draw
            renderer.Draw(va, ib, shader);

            // Change the red channel and check values
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}