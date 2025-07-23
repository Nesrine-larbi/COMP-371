#include "../include/Sun.h"

Sun::Sun() {
    setScale(glm::vec3(20.0f)); // Sun is larger than planets
    setRotationSpeed(0.5f); // Slow rotation
    
    // Initialize sphere, shaders, and texture
    generateSphere();
    setupShaders(getVertexShader(), getFragmentShader());
    loadTexture(getTexturePath());
}

std::string Sun::getFragmentShader() {
    return R"(
        #version 330 core
        out vec4 FragColor;
        in vec2 TexCoord;
        uniform sampler2D objectTexture;
        void main() {
            vec4 texColor = texture(objectTexture, TexCoord);
            if (texColor.rgb == vec3(0.0)) {
                FragColor = vec4(1.0, 0.8, 0.0, 1.0); // Fallback golden color
            } else {
                // Add slight glow effect
                FragColor = texColor * 1.2;
            }
        }
    )";
}
