#define GLEW_STATIC 1
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "../include/LightVisualizer.h"

LightVisualizer::LightVisualizer() : indexCount(0), sphereVAO(0), sphereVBO(0), sphereEBO(0) {
    generateSmallSphere();
    // Load shaders from files
    shader = std::unique_ptr<Shader>(Shader::fromFiles("shaders/light_visualizer.vert", "shaders/light_visualizer.frag"));
    if (!shader) {
        std::cerr << "Failed to load light visualizer shaders" << std::endl;
    }
}

LightVisualizer::~LightVisualizer() {
    if (sphereVAO) glDeleteVertexArrays(1, &sphereVAO);
    if (sphereVBO) glDeleteBuffers(1, &sphereVBO);
    if (sphereEBO) glDeleteBuffers(1, &sphereEBO);
}

void LightVisualizer::generateSmallSphere() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    const float PI = 3.14159265359f;
    int sectorCount = 12;
    int stackCount = 8;
    
    // Generate vertices for a small sphere
    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = PI / 2 - i * PI / stackCount;
        float xy = cos(stackAngle);
        float z = sin(stackAngle);
        
        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * PI / sectorCount;
            
            vertices.push_back(xy * cos(sectorAngle));
            vertices.push_back(xy * sin(sectorAngle));
            vertices.push_back(z);
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
    
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);
    
    glBindVertexArray(sphereVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void LightVisualizer::render(const glm::mat4& view, const glm::mat4& projection) {
    // Empty implementation - use renderLights instead
}

void LightVisualizer::renderLights(const glm::mat4& view, const glm::mat4& projection, 
                                  const LightManager& lightManager) {
    shader->use();
    
    glBindVertexArray(sphereVAO);
    
    for (size_t i = 0; i < lightManager.getLightCount(); ++i) {
        const Light& light = lightManager.getLight(i);
        
        // Skip the sun light (index 0) as it's already visible
        if (i == 0) continue;
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, light.getPosition());
        model = glm::scale(model, glm::vec3(2.0f)); // Small light indicators
        
        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setVec3("lightColor", light.getDiffuse());
        
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
}
