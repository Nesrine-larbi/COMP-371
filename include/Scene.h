#pragma once

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "SceneObject.h"
#include "CelestialBody.h" 
#include <vector>
#include <memory>

// Forward declarations
class Skybox;
class Sun;
class Earth;
class Mars;
class Venus;

class Scene {
private:
    GLFWwindow* window;
    Camera camera;
    std::vector<std::unique_ptr<SceneObject> > objects;  // C++98 compatible spacing
    std::vector<std::unique_ptr<CelestialBody> > celestialBodies; // Separate container for celestial objects
    
    float deltaTime;
    float lastFrame;
    double lastX, lastY;
    bool firstMouse;
    
    static Scene* instance;

public:
    Scene();
    
    bool initialize();
    void run();
    void cleanup();
    void processInput();
    
    // Add methods to manage celestial bodies
    void addCelestialBody(std::unique_ptr<CelestialBody> body);
    
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};
