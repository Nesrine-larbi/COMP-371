#include "../include/ShadowMap.h"
#include <iostream>

ShadowMap::ShadowMap(unsigned int width, unsigned int height) 
    : shadowWidth(width), shadowHeight(height), depthMapFBO(0), depthMap(0) {
}

ShadowMap::~ShadowMap() {
    cleanup();
}

void ShadowMap::initialize() {
    // Create shadow shader
    shadowShader = std::unique_ptr<Shader>(Shader::fromFiles("shaders/shadow_map.vert", "shaders/shadow_map.frag"));
    
    // Generate depth map FBO
    glGenFramebuffers(1, &depthMapFBO);
    
    // Create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    // Attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR: Shadow map framebuffer not complete!" << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    std::cout << "Shadow map initialized with size: " << shadowWidth << "x" << shadowHeight << std::endl;
}

void ShadowMap::bindForWriting() {
    glViewport(0, 0, shadowWidth, shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::bindForReading(unsigned int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, depthMap);
}

glm::mat4 ShadowMap::calculateLightSpaceMatrix(const glm::vec3& lightDirection, 
                                               const glm::vec3& sceneCenter, 
                                               float sceneRadius) const {
    // Create an orthogonal projection for directional light
    float orthoSize = sceneRadius;
    glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 1.0f, orthoSize * 3.0f);
    
    // Position the light far away in the opposite direction
    glm::vec3 lightPos = sceneCenter - lightDirection * (orthoSize * 2.0f);
    glm::vec3 lightTarget = sceneCenter;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // If light direction is too close to up vector, use different up vector
    if (abs(glm::dot(lightDirection, up)) > 0.9f) {
        up = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    
    glm::mat4 lightView = glm::lookAt(lightPos, lightTarget, up);
    
    return lightProjection * lightView;
}

void ShadowMap::beginShadowPass(const glm::mat4& lightSpaceMatrix) {
    bindForWriting();
    shadowShader->use();
    shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
}

void ShadowMap::endShadowPass() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::cleanup() {
    if (depthMapFBO != 0) {
        glDeleteFramebuffers(1, &depthMapFBO);
        depthMapFBO = 0;
    }
    if (depthMap != 0) {
        glDeleteTextures(1, &depthMap);
        depthMap = 0;
    }
}
