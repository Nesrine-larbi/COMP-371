#pragma once

#include <glm/glm.hpp>

// Base SceneObject class
class SceneObject {
protected:
    glm::vec3 position{0.0f};
    glm::vec3 scale{1.0f};
    unsigned int VAO = 0, VBO = 0, EBO = 0;

public:
    virtual ~SceneObject();
    
    virtual void render(const glm::mat4& view, const glm::mat4& projection) = 0;
    
    void setPosition(const glm::vec3& pos);
    void setScale(const glm::vec3& s);
};
