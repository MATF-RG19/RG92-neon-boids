//
// Created by rentorious on 08/09/2020.
//

#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string &filepath) :
        m_FilePath(filepath), m_RendererID() {
    // Parse shader code
    ShaderProgramSource source = ParseShader(filepath);
    // Create Vertex and Fragment shaders
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

// This function parses a shader file. It extracts the vertex and fragment shader source codes from the file
ShaderProgramSource Shader::ParseShader(const std::string &filepath) {
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

unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {
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

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
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

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

// Set uniform
void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    // Get uniform location

    // Set uniform color
    GLCall(glUniform4f(GetUnifromLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string &name, float value) {
    GLCall(glUniform1f(GetUnifromLocation(name), value));
}

int Shader::GetUnifromLocation(const std::string &name) {
    // Check if cached
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    // Get location
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    // Check if valid
    if (location == -1)
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;

    // Cache the Uniform location
    m_UniformLocationCache[name] = location;

    return location;
}