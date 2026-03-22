#ifndef SHADER_H
#define SHADER_H

#include <string>

unsigned int LoadShaderProgram(
    const std::string& vertexPath,
    const std::string& fragmentPath
);

#endif