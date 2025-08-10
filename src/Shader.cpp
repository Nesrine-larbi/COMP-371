#define GLEW_STATIC 1
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "../include/Shader.h"

std::string Shader::readShaderFile(const std::string& filePath) {
    std::ifstream shaderFile;
    std::stringstream shaderStream;
    
    // Enable exceptions for ifstream
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        // Open files
        shaderFile.open(filePath);
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        
        // Convert stream into string
        std::string result = shaderStream.str();
        std::cout << "Successfully read shader file: " << filePath << " (" << result.length() << " characters)" << std::endl;
        return result;
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filePath << std::endl;
        std::cerr << "Error: " << e.what() << std::endl;
        return "";
    }
}


unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    if (source.empty()) {
        std::cerr << "ERROR::SHADER::EMPTY_SOURCE" << std::endl;
        return 0;
    }
    
    unsigned int shader = glCreateShader(type);
    if (shader == 0) {
        std::cerr << "ERROR::SHADER::CREATION_FAILED" << std::endl;
        return 0;
    }
    
    const char* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);
    
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::string shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::cerr << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED" << std::endl;
        std::cerr << "Error log: " << infoLog << std::endl;
        std::cerr << "Shader source (" << source.length() << " chars):" << std::endl;
        std::cerr << "----------------------------------------" << std::endl;
        std::cerr << source << std::endl;
        std::cerr << "----------------------------------------" << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    
    std::cout << "Successfully compiled " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader" << std::endl;
    return shader;
}

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) {
    // Handle empty sources (used by fromFiles method)
    if (vertexSource.empty() || fragmentSource.empty()) {
        programID = 0; // Initialize with invalid program ID
        return;
    }
    
    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    
    if (vertex == 0 || fragment == 0) {
        programID = 0;
        return;
    }
    
    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    
    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
        std::cerr << "Link error log: " << infoLog << std::endl;
        glDeleteProgram(programID);
        programID = 0;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader* Shader::fromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    Shader* shader = new Shader("", ""); // Create with empty strings (programID will be 0)
    
    // Read shader source code from files
    std::string vertexSource = shader->readShaderFile(vertexPath);
    std::string fragmentSource = shader->readShaderFile(fragmentPath);
    
    if (vertexSource.empty() || fragmentSource.empty()) {
        std::cerr << "ERROR::SHADER::FILE_LOADING_FAILED" << std::endl;
        std::cerr << "Vertex source empty: " << vertexSource.empty() << ", Fragment source empty: " << fragmentSource.empty() << std::endl;
        delete shader;
        return nullptr;
    }
    
    // No need to delete programID since it was never created (is 0)
    
    unsigned int vertex = shader->compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragment = shader->compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    
    if (vertex == 0 || fragment == 0) {
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED" << std::endl;
        std::cerr << "Vertex shader ID: " << vertex << ", Fragment shader ID: " << fragment << std::endl;
        if (vertex != 0) glDeleteShader(vertex);
        if (fragment != 0) glDeleteShader(fragment);
        delete shader;
        return nullptr;
    }
    
    shader->programID = glCreateProgram();
    glAttachShader(shader->programID, vertex);
    glAttachShader(shader->programID, fragment);
    glLinkProgram(shader->programID);
    
    int success;
    glGetProgramiv(shader->programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(shader->programID, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
        std::cerr << "Link error log: " << infoLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        delete shader;
        return nullptr;
    }
    
    std::cout << "Successfully linked shader program from files: " << vertexPath << " and " << fragmentPath << std::endl;
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return shader;
}

Shader::~Shader() {
    if (programID != 0) {
        glDeleteProgram(programID);
    }
}

void Shader::use() const { 
    glUseProgram(programID); 
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &vec[0]);
}
