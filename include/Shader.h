#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
private:
    unsigned int programID;
    
    unsigned int compileShader(unsigned int type, const std::string& source);

public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    ~Shader();
    
    // Delete copy constructor and assignment
    Shader(const Shader&);
    Shader& operator=(const Shader&);
    
    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setInt(const std::string& name, int value) const;
};
