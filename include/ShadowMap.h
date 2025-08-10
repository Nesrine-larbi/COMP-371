#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <memory>

class ShadowMap {
private:
    unsigned int depthMapFBO;
    unsigned int depthMap;
    unsigned int shadowWidth, shadowHeight;
    std::unique_ptr<Shader> shadowShader;
    
public:
    ShadowMap(unsigned int width = 2048, unsigned int height = 2048);
    ~ShadowMap();
    
    void initialize();
    void bindForWriting();
    void bindForReading(unsigned int textureUnit = 0) const;
    
    // Calculate light space matrix for directional lights
    glm::mat4 calculateLightSpaceMatrix(const glm::vec3& lightDirection, 
                                       const glm::vec3& sceneCenter, 
                                       float sceneRadius = 200.0f) const;
    
    // Begin shadow pass
    void beginShadowPass(const glm::mat4& lightSpaceMatrix);
    void endShadowPass();
    
    Shader* getShadowShader() const { return shadowShader.get(); }
    unsigned int getDepthMap() const { return depthMap; }
    
    void cleanup();
};
