#include "../include/Light.h"
#include <cmath>

Light::Light(LightType lightType) 
    : type(lightType), position(0.0f), direction(0.0f, -1.0f, 0.0f),
      ambient(0.1f), diffuse(0.5f), specular(1.0f),
      constant(1.0f), linear(0.09f), quadratic(0.032f),
      cutOff(12.5f), outerCutOff(17.5f),
      movementCenter(0.0f), movementRadius(0.0f), movementSpeed(0.0f),
      movementAngle(0.0f), isMoving(false) {
}

void Light::setMovement(const glm::vec3& center, float radius, float speed) {
    movementCenter = center;
    movementRadius = radius;
    movementSpeed = speed;
    isMoving = true;
}

void Light::update(float deltaTime) {
    if (isMoving && movementRadius > 0.0f) {
        movementAngle += movementSpeed * deltaTime;
        if (movementAngle > 2.0f * 3.14159265359f) {
            movementAngle -= 2.0f * 3.14159265359f;
        }
        
        // Calculate new position in circular motion
        position = movementCenter + glm::vec3(
            movementRadius * cos(movementAngle),
            0.0f,
            movementRadius * sin(movementAngle)
        );
    }
}

Light Light::createSunLight(const glm::vec3& position) {
    Light light(LIGHT_POINT);
    light.setPosition(position);
    light.setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
    light.setDiffuse(glm::vec3(1.0f, 0.9f, 0.7f));
    light.setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
    light.setAttenuation(1.0f, 0.0014f, 0.000007f); // Very long range for sun
    return light;
}

Light Light::createMovingColoredLight(const glm::vec3& center, float radius, 
                                     const glm::vec3& color, float speed) {
    Light light(LIGHT_POINT);
    light.setMovement(center, radius, speed);
    light.setAmbient(color * 0.1f);
    light.setDiffuse(color * 0.8f);
    light.setSpecular(color);
    light.setAttenuation(1.0f, 0.09f, 0.032f);
    return light;
}

Light Light::createSpotlight(const glm::vec3& position, const glm::vec3& direction,
                            float innerCone, float outerCone) {
    Light light(LIGHT_SPOT);
    light.setPosition(position);
    light.setDirection(direction);
    light.setAmbient(glm::vec3(0.0f));
    light.setDiffuse(glm::vec3(1.0f));
    light.setSpecular(glm::vec3(1.0f));
    light.setAttenuation(1.0f, 0.09f, 0.032f);
    light.setSpotlight(innerCone, outerCone);
    return light;
}

Light Light::createPlanetLight(const glm::vec3& sunPosition, const glm::vec3& planetPosition, 
                              const glm::vec3& lightColor) {
    Light light(LIGHT_POINT); // Use point light instead of directional for distance-based illumination
    
    // Calculate direction from sun to planet
    glm::vec3 sunToPlanet = planetPosition - sunPosition;
    float distanceFromSun = glm::length(sunToPlanet);
    glm::vec3 direction = glm::normalize(sunToPlanet);
    
    // Position light at some distance from the planet towards the sun
    float lightDistance = 15.0f; // Distance from planet surface
    glm::vec3 lightPosition = planetPosition - (direction * lightDistance);
    
    // Calculate light intensity based on distance from sun (inverse square law)
    float baseSunDistance = 50.0f; // Reference distance for full intensity
    float intensityFactor = (baseSunDistance * baseSunDistance) / (distanceFromSun * distanceFromSun);
    intensityFactor = glm::clamp(intensityFactor, 0.1f, 2.0f); // Clamp to reasonable range
    
    light.setPosition(lightPosition);
    light.setDirection(direction); // Point towards planet
    light.setAmbient(lightColor * 0.1f * intensityFactor); 
    light.setDiffuse(lightColor * 0.8f * intensityFactor);   
    light.setSpecular(lightColor * 0.9f * intensityFactor);  
    
    // Set attenuation for point light (stronger for closer planets)
    float constant = 1.0f;
    float linear = 0.014f / intensityFactor; // Adjust attenuation based on distance from sun
    float quadratic = 0.0007f / intensityFactor;
    light.setAttenuation(constant, linear, quadratic);
    
    return light;
}
