#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum LightType {
    LIGHT_DIRECTIONAL,
    LIGHT_POINT,
    LIGHT_SPOT
};

class Light {
private:
    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    // For point and spot lights
    float constant;
    float linear;
    float quadratic;
    
    // For spot lights
    float cutOff;
    float outerCutOff;
    
    // Movement properties
    glm::vec3 movementCenter;
    float movementRadius;
    float movementSpeed;
    float movementAngle;
    bool isMoving;

public:
    Light(LightType lightType = LIGHT_POINT);
    
    // Setters
    void setPosition(const glm::vec3& pos) { position = pos; }
    void setDirection(const glm::vec3& dir) { direction = glm::normalize(dir); }
    void setAmbient(const glm::vec3& amb) { ambient = amb; }
    void setDiffuse(const glm::vec3& diff) { diffuse = diff; }
    void setSpecular(const glm::vec3& spec) { specular = spec; }
    void setAttenuation(float c, float l, float q) { constant = c; linear = l; quadratic = q; }
    void setSpotlight(float inner, float outer) { cutOff = inner; outerCutOff = outer; }
    
    // Movement configuration
    void setMovement(const glm::vec3& center, float radius, float speed);
    void enableMovement(bool enable) { isMoving = enable; }
    
    // Getters
    LightType getType() const { return type; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getDirection() const { return direction; }
    glm::vec3 getAmbient() const { return ambient; }
    glm::vec3 getDiffuse() const { return diffuse; }
    glm::vec3 getSpecular() const { return specular; }
    float getConstant() const { return constant; }
    float getLinear() const { return linear; }
    float getQuadratic() const { return quadratic; }
    float getCutOff() const { return cutOff; }
    float getOuterCutOff() const { return outerCutOff; }
    
    // Update light position if moving
    void update(float deltaTime);
    
    // Factory methods for common light types
    static Light createSunLight(const glm::vec3& position);
    static Light createMovingColoredLight(const glm::vec3& center, float radius, 
                                         const glm::vec3& color, float speed);
    static Light createSpotlight(const glm::vec3& position, const glm::vec3& direction,
                                float innerCone, float outerCone);
    static Light createPlanetLight(const glm::vec3& sunPosition, const glm::vec3& planetPosition, 
                                  const glm::vec3& lightColor = glm::vec3(1.0f, 0.95f, 0.8f));
};
