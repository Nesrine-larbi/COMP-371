#define GLEW_STATIC 1
#include <GL/glew.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "../include/stb_image.h"
#include "../include/Skybox.h"

void Skybox::setupMesh() {
    float skyboxVertices[] = {
        -1000.0f,  1000.0f, -1000.0f,  -1000.0f, -1000.0f, -1000.0f,   1000.0f, -1000.0f, -1000.0f,
         1000.0f, -1000.0f, -1000.0f,   1000.0f,  1000.0f, -1000.0f,  -1000.0f,  1000.0f, -1000.0f,
        -1000.0f, -1000.0f,  1000.0f,  -1000.0f, -1000.0f, -1000.0f,  -1000.0f,  1000.0f, -1000.0f,
        -1000.0f,  1000.0f, -1000.0f,  -1000.0f,  1000.0f,  1000.0f,  -1000.0f, -1000.0f,  1000.0f,
         1000.0f, -1000.0f, -1000.0f,   1000.0f, -1000.0f,  1000.0f,   1000.0f,  1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,   1000.0f,  1000.0f, -1000.0f,   1000.0f, -1000.0f, -1000.0f,
        -1000.0f, -1000.0f,  1000.0f,  -1000.0f,  1000.0f,  1000.0f,   1000.0f,  1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,   1000.0f, -1000.0f,  1000.0f,  -1000.0f, -1000.0f,  1000.0f,
        -1000.0f,  1000.0f, -1000.0f,   1000.0f,  1000.0f, -1000.0f,   1000.0f,  1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,  -1000.0f,  1000.0f,  1000.0f,  -1000.0f,  1000.0f, -1000.0f,
        -1000.0f, -1000.0f, -1000.0f,  -1000.0f, -1000.0f,  1000.0f,   1000.0f, -1000.0f, -1000.0f,
         1000.0f, -1000.0f, -1000.0f,  -1000.0f, -1000.0f,  1000.0f,   1000.0f, -1000.0f,  1000.0f
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Skybox::loadCubemap() {
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load("assets/skybox/space2.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        for (unsigned int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load skybox texture" << std::endl;
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Skybox::Skybox() {
    std::string vertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        out vec3 TexCoords;
        uniform mat4 projection;
        uniform mat4 view;
        void main() {
            TexCoords = aPos;
            vec4 pos = projection * view * vec4(aPos, 1.0);
            gl_Position = pos.xyww;
        }
    )";
    
    std::string fragmentShader = R"(
        #version 330 core
        out vec4 FragColor;
        in vec3 TexCoords;
        uniform samplerCube skybox;
        void main() {
            FragColor = texture(skybox, TexCoords);
        }
    )";
    
    shader = std::make_unique<Shader>(vertexShader, fragmentShader);
    setupMesh();
    loadCubemap();
}

Skybox::~Skybox() {
    if (cubemapTexture) glDeleteTextures(1, &cubemapTexture);
}

void Skybox::render(const glm::mat4& view, const glm::mat4& projection) {
    glDepthFunc(GL_LEQUAL);
    shader->use();
    
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
    shader->setMat4("view", skyboxView);
    shader->setMat4("projection", projection);
    
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glDepthFunc(GL_LESS);
}