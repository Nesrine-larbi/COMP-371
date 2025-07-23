#pragma once

#include "SceneObject.h"
#include "Shader.h"
#include <memory>

class Skybox : public SceneObject {
private:
    std::unique_ptr<Shader> shader;
    unsigned int cubemapTexture;
    
    void setupMesh();
    void loadCubemap();

public:
    Skybox();
    ~Skybox();
    
    void render(const glm::mat4& view, const glm::mat4& projection);
};
