#include "../include/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

Camera::Camera(glm::vec3 pos) 
    : position(pos), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)) {
    yaw = -90.0f;
    pitch = 0.0f;
    speed = 5.0f;
    sensitivity = 0.1f;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(int direction, float deltaTime) {
    float velocity = speed * deltaTime;
    switch(direction) {
        case 0: position += front * velocity; break;  // W
        case 1: position -= front * velocity; break;  // S  
        case 2: position -= right * velocity; break;  // A
        case 3: position += right * velocity; break;  // D
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw += xoffset;
    pitch += yoffset;
    
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    updateCameraVectors();
}
