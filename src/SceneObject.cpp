#define GLEW_STATIC 1
#include <GL/glew.h>
#include "../include/SceneObject.h"

SceneObject::~SceneObject() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
}

void SceneObject::setPosition(const glm::vec3& pos) { 
    position = pos; 
}

void SceneObject::setScale(const glm::vec3& s) { 
    scale = s; 
}
