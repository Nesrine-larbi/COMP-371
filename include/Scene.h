#pragma once

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "SceneObject.h"
#include "CelestialBody.h" 
#include "LightManager.h"
#include "LightVisualizer.h"
#include "ShadowMap.h"
#include "Shader.h"
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
    LightManager lightManager;
    std::unique_ptr<LightVisualizer> lightVisualizer;
    std::unique_ptr<ShadowMap> shadowMap;
    std::unique_ptr<Shader> satelliteShader;
    std::vector<std::unique_ptr<SceneObject> > objects;  // C++98 compatible spacing
    std::vector<std::unique_ptr<CelestialBody> > celestialBodies; // Separate container for celestial objects
    std::vector<size_t> planetLightIndices; // Track which lights belong to which planets
    
    int windowWidth;
    int windowHeight;
    
    float deltaTime;
    float lastFrame;
    double lastX, lastY;
    bool firstMouse;
    bool showLightVisualizer;
    bool enableDynamicLighting;
    bool enableShadows;
    bool isSpinning;
    
    static Scene* instance;

public:
    Scene();
    
    bool initialize();
    void run();
    void cleanup();
    void processInput();
    
    void addCelestialBody(std::unique_ptr<CelestialBody> body);
    void createPlanetLightsSystem();
    void updatePlanetLights();
    void renderShadowPass();
    glm::vec3 calculateSunToEarthDirection();
    
    
    void setWindowSize(int width, int height);
    
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};
