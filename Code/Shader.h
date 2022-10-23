#pragma once
#include <map>
#include <string>

class Shader
{
public:
    unsigned int handle;

    std::map<std::string, unsigned int> atributes;
    std::map<std::string, unsigned int> uniforms;

    Shader();
    Shader(const std::string& vertex, const std::string& fragment);
    ~Shader();
    
private:
    // This class holds a handle to GPU resource, so copying and assigning should be disabled
    Shader(const Shader&);
    Shader& operator=(const Shader&);

public:
    void Load(const std::string& vertex, const std::string& fragment);
    void Bind();
    void UnBind();

    unsigned int GetAttribute(const std::string& name);
    unsigned int GetUniform(const std::string& name);
    unsigned int GetHandle();
    
    
private:

    std::string ReadFile(const std::string& path);
    unsigned int CompileVertexShader(const std::string& vertex);
    unsigned int CompileFragmentShader(const std::string& fragment);
    bool LinkShaders(unsigned int vertex, unsigned int fragment);

    void PopulateAttributes();
    void PopulateUniforms();
};
