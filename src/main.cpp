#define GLEW_STATIC

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <csignal>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"


// Struct for returning shader source strings
struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

// This function parses a shader file. It extracts the vertex and fragment shader source codes from the file
static ShaderProgramSource ParseShader(const std::string &filepath) {
    // Stream for reading the shader source
    std::ifstream stream(filepath);

    // Enum for writing into different shader buffers
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        // Check if the line contains a marker for the new shader code
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                // set mode to vertex
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        } else {
            // Write shader line of code into the correct buffer
            ss[(int) type] << line << "\n";
        }
    }

    return {ss[(int) ShaderType::VERTEX].str(), ss[(int) ShaderType::FRAGMENT].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string &source) {
    // Create shader object of the specified shader type
    GLCall(unsigned int id = glCreateShader(type));
    // Get the c string from the shader source code because OpenGL requires it
    const char *src = source.c_str();
    // Add the source code to the shader object
    GLCall(glShaderSource(id, 1, &src, nullptr));
    // Compile the shader
    GLCall(glCompileShader(id));

    // Check if shader compilation was successful
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        // Get the failure message
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char *message = (char *) alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        // Print the message
        std::cout << "Failed to compile " << ((type == GL_VERTEX_SHADER) ? "vertex " : "fragment ") << "shader!"
                  << std::endl;
        std::cout << message << std::endl;

        // Get rid of the invalid shader
        GLCall(glDeleteShader(id));

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    // Mate the program and shader OpenGL objects
    GLCall(unsigned int program = glCreateProgram());
    GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    // Attach shaders to the program and then compile and link the program
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    // Delete shares because they are now attached to the program and the original objects are no longer needed
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

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
                -0.5f, -0.5f, // 0
                0.5f, -0.5, // 1
                0.5f, 0.5f, // 2
                -0.5f, 0.5f, // 3
        };

        unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        // Vertex array object
        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        // VertexArray instance
        VertexArray va;
        // Vertex Buffer
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        va.AddBuffer(vb);

        BufferLayout layout;
        layout.push<float>(3);
        va.AddLayout(layout);

        // Attribute buffer
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));

        // Index buffer
        IndexBuffer ib(indices, 6);

        // Parse shader code
        GLCall(ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"));
        // Create Vertex and Fragment shaders
        GLCall(int shader = CreateShader(source.VertexSource, source.FragmentSource));
        // Bind the shaders
        GLCall(glUseProgram(shader));

        // Get uniform id
        GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);
        // Set uniform color
        GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

        // Unbind all
        GLCall(glBindVertexArray(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        GLCall(glUseProgram(0));


        // Color channel variables for animating the rectangle
        float r = 0.0f;
        float increment = 0.05f;


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            // bind shaders
            GLCall(glUseProgram(shader));
            // Set Rectangle color
            GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
            // bind vertex array
            GLCall(glBindVertexArray(vao));
            // Bind Indices buffer
            ib.Bind();
            // Draw the rectangle
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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

        GLCall(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}