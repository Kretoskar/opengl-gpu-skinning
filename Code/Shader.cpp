#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Core/glad.h"

Shader::Shader()
{
    handle = glCreateProgram();
}

Shader::Shader(const std::string& vertex, const std::string& fragment)
{
    handle = glCreateProgram();
    Load(vertex, fragment);
}

Shader::~Shader()
{
    glDeleteProgram(handle);
}

void Shader::Load(const std::string& vertex, const std::string& fragment)
{
    std::ifstream f(vertex.c_str());
    bool vertFile = f.good();
    f.close();

    f = std::ifstream(vertex.c_str());
    bool fragFile = f.good();
    f.close();

    std::string v_source = vertex;
    if (vertFile) {
        v_source = ReadFile(vertex);
    }

    std::string f_source = fragment;
    if (fragFile) {
        f_source = ReadFile(fragment);
    }

    unsigned int v_shader = CompileVertexShader(v_source);
    unsigned int f_shader = CompileFragmentShader(f_source);
    if (LinkShaders(v_shader, f_shader)) {
        PopulateAttributes();
        PopulateUniforms();
    }
}

void Shader::Bind()
{
    glUseProgram(handle);
}

void Shader::UnBind()
{
    glUseProgram(0);
}

unsigned Shader::GetAttribute(const std::string& name)
{
    std::map<std::string, unsigned int>::iterator it = atributes.find(name);
    if (it == atributes.end()) {
        std::cout << "Retrieving bad attribute index: " << name << "\n";
        return 0;
    }
    return it->second;
}

unsigned Shader::GetUniform(const std::string& name)
{
    std::map<std::string, unsigned int>::iterator it = uniforms.find(name);
    if (it == uniforms.end()) {
        std::cout << "Retrieving bad uniform index: " << name << "\n";
        return 0;
    }
    return it->second;
}


unsigned Shader::GetHandle()
{
    return handle;
}

std::string Shader::ReadFile(const std::string& path)
{
    std::ifstream file;
    file.open(path);
    std::stringstream contents;
    contents << file.rdbuf();
    file.close();
    return contents.str();
}

unsigned Shader::CompileVertexShader(const std::string& vertex)
{
    unsigned int v = glCreateShader(GL_VERTEX_SHADER);
    const char* vSource = vertex.c_str();
    glShaderSource(v, 1, &vSource, nullptr);
    glCompileShader(v);
    int success = 0;
    glGetShaderiv(v, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(v, 512, nullptr, infoLog);
        std::cout << "Vertex compilation failed. \n";
        std::cout << "\t" << infoLog << "\n";
        glDeleteShader(v);
        return 0;
    }
    return v;
}

unsigned Shader::CompileFragmentShader(const std::string& fragment)
{
    unsigned int f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* f_source = fragment.c_str();
    glShaderSource(f_shader, 1, &f_source, nullptr);
    glCompileShader(f_shader);
    int success = 0;
    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(f_shader, 512, nullptr, infoLog);
        std::cout << "ERROR: Fragment compilation failed.\n";
        std::cout << "\t" << infoLog << "\n";
        glDeleteShader(f_shader);
        return 0;
    };
    return f_shader;
}

bool Shader::LinkShaders(unsigned vertex, unsigned fragment)
{
    glAttachShader(handle, vertex);
    glAttachShader(handle, fragment);
    glLinkProgram(handle);
    int success = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(handle, 512, nullptr, infoLog);
        std::cout << "ERROR: Shader linking failed.\n";
        std::cout << "\t" << infoLog << "\n";
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return false;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

void Shader::PopulateAttributes()
{
    int count = -1;
    int length;
    char name[128];
    int size;
    GLenum type;

    glUseProgram(handle);
    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &count);

    for (int i = 0; i < count; ++i) {
        memset(name, 0, sizeof(char) * 128);
        glGetActiveAttrib(handle, (GLuint)i, 128, &length, &size, &type, name);
        int attrib = glGetAttribLocation(handle, name);
        if (attrib >= 0) {
            atributes[name] = attrib;
        }
    }

    glUseProgram(0);
}

void Shader::PopulateUniforms()
{
    int count = -1;
    int length;
    char name[128];
    int size;
    GLenum type;
    char testName[256];

    glUseProgram(handle);
    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; ++i) {
        memset(name, 0, sizeof(char) * 128);
        glGetActiveUniform(handle, (GLuint)i, 128, &length, &size, &type, name);

        int uniform = glGetUniformLocation(handle, name);
        if (uniform >= 0) {
            std::string uniformName = name;
            std::size_t found = uniformName.find('[');
            if (found != std::string::npos) {
                uniformName.erase(uniformName.begin() + found, uniformName.end());
                // Populate subscripted names too
                unsigned int uniformIndex = 0;
                while (true) {
                    memset(testName, 0, sizeof(char) * 256);
                    sprintf_s(testName, "%s[%d]", uniformName.c_str(), uniformIndex++);
                    int uniformLocation = glGetUniformLocation(handle, testName);
                    if (uniformLocation < 0) {
                        break;
                    }
                    uniforms[testName] = uniformLocation;
                }
            }
            uniforms[uniformName] = uniform;
        }
    }

    glUseProgram(0);
}
