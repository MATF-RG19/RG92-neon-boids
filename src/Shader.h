//
// Created by rentorious on 08/09/2020.
//

#ifndef NEONBOIDS_SHADER_H
#define NEONBOIDS_SHADER_H

#endif //NEONBOIDS_SHADER_H

#include <string>
#include <unordered_map>

// Struct for returning shader source strings
struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string &filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set uniforms
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void SetUniform1f(const std::string &name, float value);

private:
    int GetUnifromLocation(const std::string &name);
    unsigned int CompileShader(unsigned int type, const std::string &source);
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
    ShaderProgramSource ParseShader(const std::string &filepath);
};