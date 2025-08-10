#include "../include/LightManager.h"
#include <iostream>
#include <string>
#include <cmath>

LightManager::LightManager() {
}

LightManager::~LightManager() {
}

void LightManager::addLight(const Light& light) {
    if (lights.size() < MAX_LIGHTS) {
        lights.push_back(light);
    } else {
        std::cerr << "Warning: Maximum number of lights (" << MAX_LIGHTS << ") reached!" << std::endl;
    }
}

void LightManager::removeLight(size_t index) {
    if (index < lights.size()) {
        lights.erase(lights.begin() + index);
    }
}

void LightManager::clearLights() {
    lights.clear();
}

void LightManager::update(float deltaTime) {
    for (size_t i = 0; i < lights.size(); ++i) {
        lights[i].update(deltaTime);
    }
}

void LightManager::applyLightsToShader(const Shader& shader, const glm::vec3& viewPos) const {
    shader.use();
    
    // Set number of lights
    shader.setInt("numLights", static_cast<int>(lights.size()));
    shader.setVec3("viewPos", viewPos);
    
    // Apply each light
    for (size_t i = 0; i < lights.size() && i < MAX_LIGHTS; ++i) {
        const Light& light = lights[i];
        std::string base = "lights[" + std::to_string(i) + "]";
        
        shader.setInt(base + ".type", static_cast<int>(light.getType()));
        shader.setVec3(base + ".position", light.getPosition());
        shader.setVec3(base + ".direction", light.getDirection());
        shader.setVec3(base + ".ambient", light.getAmbient());
        shader.setVec3(base + ".diffuse", light.getDiffuse());
        shader.setVec3(base + ".specular", light.getSpecular());
        shader.setFloat(base + ".constant", light.getConstant());
        shader.setFloat(base + ".linear", light.getLinear());
        shader.setFloat(base + ".quadratic", light.getQuadratic());
        shader.setFloat(base + ".cutOff", cos(glm::radians(light.getCutOff())));
        shader.setFloat(base + ".outerCutOff", cos(glm::radians(light.getOuterCutOff())));
    }
}

void LightManager::setupDefaultLights() {
    clearLights();
    
    // Sun light (stationary) - this will be the main light source
    Light sunLight = Light::createSunLight(glm::vec3(0.0f, 0.0f, 0.0f));
    addLight(sunLight);
    
    // Note: Planet-specific lights will be added by the Scene class
}

void LightManager::addPlanetLight(const glm::vec3& sunPosition, const glm::vec3& planetPosition, 
                                 const glm::vec3& lightColor) {
    Light planetLight = Light::createPlanetLight(sunPosition, planetPosition, lightColor);
    addLight(planetLight);
}

void LightManager::updatePlanetLight(size_t lightIndex, const glm::vec3& sunPosition, const glm::vec3& planetPosition) {
    if (lightIndex < lights.size()) {
        // Calculate direction from sun to planet
        glm::vec3 sunToPlanet = planetPosition - sunPosition;
        float distanceFromSun = glm::length(sunToPlanet);
        glm::vec3 direction = glm::normalize(sunToPlanet);
        
        // Position light at distance from planet towards the sun
        float lightDistance = 15.0f; // Distance from planet surface
        glm::vec3 lightPosition = planetPosition - (direction * lightDistance);
        
        // Update light intensity based on current distance from sun
        float baseSunDistance = 50.0f; // Reference distance for full intensity
        float intensityFactor = (baseSunDistance * baseSunDistance) / (distanceFromSun * distanceFromSun);
        intensityFactor = glm::clamp(intensityFactor, 0.1f, 2.0f);
        
        // Update light properties
        lights[lightIndex].setPosition(lightPosition);
        lights[lightIndex].setDirection(direction);
        
        // Update intensity based on distance
        glm::vec3 baseColor = glm::vec3(1.0f, 0.95f, 0.8f); // Base sunlight color
        lights[lightIndex].setAmbient(baseColor * 0.1f * intensityFactor);
        lights[lightIndex].setDiffuse(baseColor * 0.8f * intensityFactor);
        lights[lightIndex].setSpecular(baseColor * 0.9f * intensityFactor);
        
        // Update attenuation
        float constant = 1.0f;
        float linear = 0.014f / intensityFactor;
        float quadratic = 0.0007f / intensityFactor;
        lights[lightIndex].setAttenuation(constant, linear, quadratic);
    }
}
