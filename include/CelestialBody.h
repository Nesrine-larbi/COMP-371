#pragma once

#include "SceneObject.h"
#include "Shader.h"
#include <memory>
#include <string>

// Forward declarations
class ShadowMap;

// Base class for all celestial objects (planets, sun, moons, etc.)
class CelestialBody : public SceneObject {
protected:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Shader> basicShader; // Non-lighting shader
    unsigned int texture, texVBO;
    int indexCount;
    bool lightingEnabled;
    CelestialBody* parent = nullptr;
    // Common sphere generation
    void generateSphere(int sectorCount = 36, int stackCount = 18);
    void loadTexture(const std::string& texturePath);
    void setupShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    
    // Default shader paths that can be overridden
    virtual std::string getVertexShaderPath();
    virtual std::string getFragmentShaderPath();
    virtual std::string getBasicVertexShaderPath();   // Simple non-lighting vertex shader
    virtual std::string getBasicFragmentShaderPath(); // Simple non-lighting fragment shader
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
    virtual void renderWithLighting(const glm::mat4& view, const glm::mat4& projection, 
                                   const class LightManager& lightManager, 
                                   const glm::vec3& cameraPos);
    virtual void renderWithShadows(const glm::mat4& view, const glm::mat4& projection, 
                                  const class LightManager& lightManager, 
                                  const glm::vec3& cameraPos,
                                  const class ShadowMap* shadowMap,
                                  bool enableShadows,
                                  const glm::vec3& lightDirection);
    
    // Method to switch between lighting and non-lighting shaders
    void setLightingMode(bool enableLighting);
    
    // Setters for orbital properties
    void setOrbitalRadius(float radius) { orbitalRadius = radius; }
    void setOrbitalSpeed(float speed) { orbitalSpeed = speed; }
    void setRotationSpeed(float speed) { rotationSpeed = speed; }
    void setParent(CelestialBody* a) {parent = a;}
    glm::vec3 getPosition() const { return position;}
    glm::mat4 getModelMatrix() const;
    
    // For shadow rendering
    unsigned int getVAO() const { return VAO; }
    int getIndexCount() const { return indexCount; }
    
    virtual void update(float deltaTime); // For animation/rotation
};
