#pragma once

#include <glm/glm.hpp>

class Camera {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 5.0f;
    float sensitivity = 0.1f;
    
    void updateCameraVectors();

public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 200.0f));
    
    glm::mat4 getViewMatrix() const;
    void processKeyboard(int direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
};
