#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

// helper: učitaj file u string
static std::string LoadFile(const std::string& path)
{
    std::ifstream file(path);
    std::stringstream ss;

    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return "";
    }

    ss << file.rdbuf();
    return ss.str();
}

unsigned int LoadShaderProgram(
    const std::string& vertexPath,
    const std::string& fragmentPath
)
{
    std::string vertCode = LoadFile(vertexPath);
    std::string fragCode = LoadFile(fragmentPath);

    const char* vSrc = vertCode.c_str();
    const char* fSrc = fragCode.c_str();

    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vSrc, nullptr);
    glCompileShader(vShader);

    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fSrc, nullptr);
    glCompileShader(fShader);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return program;
}