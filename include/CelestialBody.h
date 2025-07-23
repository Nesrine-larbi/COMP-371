#pragma once

#include "SceneObject.h"
#include "Shader.h"
#include <memory>
#include <string>

// Base class for all celestial objects (planets, sun, moons, etc.)
class CelestialBody : public SceneObject {
protected:
    std::unique_ptr<Shader> shader;
    unsigned int texture, texVBO;
    int indexCount;
    
    // Common sphere generation
    void generateSphere(int sectorCount = 36, int stackCount = 18);
    void loadTexture(const std::string& texturePath);
    void setupShaders(const std::string& vertexShader, const std::string& fragmentShader);
    
    // Default shaders that can be overridden
    virtual std::string getVertexShader();
    virtual std::string getFragmentShader();
    virtual std::string getTexturePath() = 0; // Pure virtual - must be implemented
    
    // Orbital properties for planets
    float orbitalRadius;
    float orbitalSpeed;
    float rotationSpeed;
    float orbitalAngle;
    float rotationAngle;
    
public:
    CelestialBody();
    virtual ~CelestialBody();
    
    virtual void render(const glm::mat4& view, const glm::mat4& projection);
    
    // Setters for orbital properties
    void setOrbitalRadius(float radius) { orbitalRadius = radius; }
    void setOrbitalSpeed(float speed) { orbitalSpeed = speed; }
    void setRotationSpeed(float speed) { rotationSpeed = speed; }
    
    virtual void update(float deltaTime); // For animation/rotation
};
