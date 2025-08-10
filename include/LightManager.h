#pragma once

#include "Light.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#define MAX_LIGHTS 8

class LightManager {
private:
    std::vector<Light> lights;
    
public:
    LightManager();
    ~LightManager();
    
    // Light management
    void addLight(const Light& light);
    void removeLight(size_t index);
    void clearLights();
    size_t getLightCount() const { return lights.size(); }
    Light& getLight(size_t index) { return lights[index]; }
    const Light& getLight(size_t index) const { return lights[index]; }
    
    // Planet-specific lighting
    void addPlanetLight(const glm::vec3& sunPosition, const glm::vec3& planetPosition, 
                       const glm::vec3& lightColor = glm::vec3(1.0f, 0.95f, 0.8f));
    void updatePlanetLight(size_t lightIndex, const glm::vec3& sunPosition, const glm::vec3& planetPosition);
    
    // Update all lights (for movement)
    void update(float deltaTime);
    
    // Apply lights to shader
    void applyLightsToShader(const Shader& shader, const glm::vec3& viewPos) const;
    
    // Setup some default dynamic lights
    void setupDefaultLights();
};
