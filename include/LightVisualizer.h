#pragma once

#include "SceneObject.h"
#include "Shader.h"
#include "LightManager.h"
#include <memory>

class LightVisualizer : public SceneObject {
private:
    std::unique_ptr<Shader> shader;
    unsigned int sphereVAO, sphereVBO, sphereEBO;
    int indexCount;
    
    void generateSmallSphere();

public:
    LightVisualizer();
    ~LightVisualizer();
    
    void render(const glm::mat4& view, const glm::mat4& projection) override;
    void renderLights(const glm::mat4& view, const glm::mat4& projection, 
                     const LightManager& lightManager);
};
