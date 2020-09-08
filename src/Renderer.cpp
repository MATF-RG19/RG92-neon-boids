//
// Created by rentorious on 07/09/2020.
//
#include "Renderer.h"
#include "IndexBuffer.h"
#include <iostream>

void GLClearError() {
    // Clear previous errors
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function, const char *file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error](" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) {
    // bind shaders
    shader.Bind();
    // bind vertex array
    va.Bind();
    // Bind Indices buffer
    ib.Bind();
    // Draw
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

}