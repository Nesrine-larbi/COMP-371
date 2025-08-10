#include "../include/Sun.h"

Sun::Sun() {
    setScale(glm::vec3(20.0f)); // Sun is larger than planets
    setRotationSpeed(0.2f); // Reduced from 0.5f - slower, smoother rotation
    
    // Explicitly ensure the Sun does NOT orbit - it stays at center
    setOrbitalRadius(0.0f);
    setOrbitalSpeed(0.0f);
    
    // Initialize sphere, shaders, and texture
    generateSphere();
    setupShaders(getVertexShaderPath(), getFragmentShaderPath());
    loadTexture(getTexturePath());
}

void Sun::update(float deltaTime) {
    // Sun should NEVER move from the center - only rotate
    setPosition(glm::vec3(0.0f, 0.0f, 0.0f)); // Force position to stay at origin
    
    // Update rotation only
    if (rotationSpeed != 0.0f) {
        rotationAngle += rotationSpeed * deltaTime;
        if (rotationAngle > 2 * 3.14159265359f) {
            rotationAngle -= 2 * 3.14159265359f;
        }
    }
}

std::string Sun::getVertexShaderPath() {
    return "shaders/sun.vert";
}

std::string Sun::getFragmentShaderPath() {
    return "shaders/sun.frag";
}
