#define GLEW_STATIC 1
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "../include/stb_image.h"
#include "../include/CelestialBody.h"
#include "../include/LightManager.h"
#include "../include/ShadowMap.h"

CelestialBody::CelestialBody() 
    : orbitalRadius(0.0f), orbitalSpeed(0.0f), rotationSpeed(0.0f), 
      orbitalAngle(0.0f), rotationAngle(0.0f), indexCount(0), 
      texture(0), texVBO(0), lightingEnabled(true) {
}

CelestialBody::~CelestialBody() {
    if (texture) glDeleteTextures(1, &texture);
    if (texVBO) glDeleteBuffers(1, &texVBO);
}

void CelestialBody::generateSphere(int sectorCount, int stackCount) {
    std::vector<float> vertices, texCoords;
    std::vector<unsigned int> indices;
    
    const float PI = 3.14159265359f;
    
    // Generate vertices
    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = PI / 2 - i * PI / stackCount;
        float xy = cos(stackAngle);
        float z = sin(stackAngle);
        
        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * PI / sectorCount;
            
            vertices.push_back(xy * cos(sectorAngle));
            vertices.push_back(xy * sin(sectorAngle));
            vertices.push_back(z);
            
            texCoords.push_back((float)j / sectorCount);
            texCoords.push_back((float)i / stackCount);
        }
    }
    
    // Generate indices
    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;
        
        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    
    indexCount = indices.size();
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &texVBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void CelestialBody::loadTexture(const std::string& texturePath) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
    stbi_image_free(data);
}

void CelestialBody::setupShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    shader = std::unique_ptr<Shader>(Shader::fromFiles(vertexShaderPath, fragmentShaderPath));
    if (!shader) {
        std::cerr << "Failed to load shaders from files: " << vertexShaderPath << " and " << fragmentShaderPath << std::endl;
        // Fallback to default behavior if file loading fails
        shader = std::unique_ptr<Shader>(new Shader("", ""));
    }
}

std::string CelestialBody::getVertexShaderPath() {
    return "shaders/celestial_body.vert";
}

std::string CelestialBody::getFragmentShaderPath() {
    return "shaders/celestial_body.frag";
}

void CelestialBody::update(float deltaTime) {
    // Update orbital position
    glm::vec3 orbitPosition(0.0f);
    if (orbitalSpeed != 0.0f) {
        orbitalAngle += orbitalSpeed * deltaTime;
        if (orbitalAngle > 2 * 3.14159265359f) {
            orbitalAngle -= 2 * 3.14159265359f;
        }
        
        // Calculate position based on orbital radius and angle
            orbitPosition = glm::vec3(
            orbitalRadius * cos(orbitalAngle),
            0.0f,
            orbitalRadius * sin(orbitalAngle)
        );
    }
    
    // Adjust position relative to parent if there's one (for the moon)
    if(parent){
        setPosition(parent->getPosition() + orbitPosition);
    }
    else 
    {
        setPosition(orbitPosition);
    }
    
    // Update rotation
    if (rotationSpeed != 0.0f) {
        rotationAngle += rotationSpeed * deltaTime;
        if (rotationAngle > 2 * 3.14159265359f) {
            rotationAngle -= 2 * 3.14159265359f;
        }
    }
}

glm::mat4 CelestialBody::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    return model;
}

void CelestialBody::render(const glm::mat4& view, const glm::mat4& projection) {
    shader->use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis rotation
    model = glm::scale(model, scale);
    
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader->setInt("objectTexture", 0);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void CelestialBody::renderWithLighting(const glm::mat4& view, const glm::mat4& projection, 
                                      const LightManager& lightManager, 
                                      const glm::vec3& cameraPos) {
    shader->use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis rotation
    model = glm::scale(model, scale);
    
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    
    // Apply lighting
    lightManager.applyLightsToShader(*shader, cameraPos);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader->setInt("objectTexture", 0);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void CelestialBody::renderWithShadows(const glm::mat4& view, const glm::mat4& projection, 
                                     const LightManager& lightManager, 
                                     const glm::vec3& cameraPos,
                                     const ShadowMap* shadowMap,
                                     bool enableShadows,
                                     const glm::vec3& lightDirection) {
    // Use shadow-enabled shader if shadows are enabled, otherwise fall back to regular lighting
    Shader* shaderToUse = shader.get();
    
    shaderToUse->use();
    
    glm::mat4 model = getModelMatrix();
    
    shaderToUse->setMat4("model", model);
    shaderToUse->setMat4("view", view);
    shaderToUse->setMat4("projection", projection);
    
    // Apply lighting
    lightManager.applyLightsToShader(*shaderToUse, cameraPos);
    
    // Set shadow-related uniforms (if shader supports them)
    if (enableShadows && shadowMap) {
        // Use the light direction passed from Scene for consistency
        glm::mat4 lightSpaceMatrix = shadowMap->calculateLightSpaceMatrix(lightDirection, glm::vec3(0.0f), 150.0f);
        
        shaderToUse->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        shaderToUse->setInt("enableShadows", 1);
        
        // Bind shadow map to texture unit 1
        shadowMap->bindForReading(1);
        shaderToUse->setInt("shadowMap", 1);
    } else {
        shaderToUse->setInt("enableShadows", 0);
    }
    
    // Bind object texture to unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shaderToUse->setInt("objectTexture", 0);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

std::string CelestialBody::getBasicVertexShaderPath() {
    return "shaders/basic.vert";
}

std::string CelestialBody::getBasicFragmentShaderPath() {
    return "shaders/basic.frag";
}

void CelestialBody::setLightingMode(bool enableLighting) {
    lightingEnabled = enableLighting;
    
    // Create appropriate shader based on lighting mode
    if (lightingEnabled) {
        shader = std::unique_ptr<Shader>(Shader::fromFiles(getVertexShaderPath(), getFragmentShaderPath()));
    } else {
        shader = std::unique_ptr<Shader>(Shader::fromFiles(getBasicVertexShaderPath(), getBasicFragmentShaderPath()));
    }
    
    if (!shader) {
        std::cerr << "Failed to load shaders for lighting mode: " << enableLighting << std::endl;
    }
}
