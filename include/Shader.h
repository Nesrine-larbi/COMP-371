#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
    unsigned int programID;
    
    unsigned int compileShader(unsigned int type, const std::string& source);
    std::string readShaderFile(const std::string& filePath);

public:
    // Constructor that takes shader source code directly
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    
    // Constructor that reads shaders from files
    static Shader* fromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    
    ~Shader();
    
    // Delete copy constructor and assignment
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    
    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& vec) const;
    void setFloat(const std::string& name, float value) const;
    void setInt(const std::string& name, int value) const;
};
