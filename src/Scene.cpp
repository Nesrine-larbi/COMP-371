#define GLEW_STATIC 1
#include <GL/glew.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <iostream>

#include "../include/Scene.h"
#include "../include/Sun.h"
#include "../include/Skybox.h"
#include "../include/Planet.h"
#include "../include/OBJloader.h"
#include "../include/OBJloaderV2.h"
#include "../include/Shader.h"

using namespace glm;
using namespace std;



GLuint setupModelVBO(string path, int &vertexCount)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;

    loadOBJ(path.c_str(), vertices, normals, UVs);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);

    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    vertexCount = vertices.size();
    return VAO;
}

GLuint setupModelEBO(string path, int &vertexCount)
{
    vector<int> vertexIndices;
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> UVs;

    loadOBJ2(path.c_str(), vertexIndices, vertices, normals, UVs);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);

    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(2);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(int), &vertexIndices.front(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    vertexCount = vertexIndices.size();
    return VAO;
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

Scene *Scene::instance = NULL;

Scene::Scene() : camera(glm::vec3(0.0f, 0.0f, 100.0f)), windowWidth(1920), windowHeight(800),
                 deltaTime(0.0f), lastFrame(0.0f), lastX(960), lastY(400), firstMouse(true),
                 showLightVisualizer(true), enableDynamicLighting(true), enableShadows(true), isSpinning(false)
{
    instance = this;
}

bool Scene::initialize()
{
    // Initialize GLFW
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, "Modern Solar System - Modular Architecture", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
        return false;

    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set initial viewport
    glViewport(0, 0, windowWidth, windowHeight);

    // Setup lighting system
    lightManager.setupDefaultLights();
    
    // Create light visualizer
    lightVisualizer = std::unique_ptr<LightVisualizer>(new LightVisualizer());
    
    // Create shadow map system
    shadowMap = std::unique_ptr<ShadowMap>(new ShadowMap());
    shadowMap->initialize();

    // Create scene objects
    objects.push_back(std::make_unique<Skybox>());
    addCelestialBody(std::make_unique<Sun>());

    // Test adding one planet at a time




    // Earth
    addCelestialBody(std::make_unique<Earth>());
    CelestialBody *earth = celestialBodies.back().get();
    addCelestialBody(std::make_unique<Moon>(earth));

    // Mars
    //addCelestialBody(std::make_unique<Mars>());
    // CelestialBody *mars = celestialBodies.back().get();
    // addCelestialBody(std::make_unique<Moon>(mars, 9.4f, 1.0f, 1.0f, 1.0f));
    // addCelestialBody(std::make_unique<Moon>(mars, 23.5f, 0.25f, 0.25f, 0.6f));

    // // Jupiter
    // addCelestialBody(std::make_unique<Jupiter>());

    // // Saturn
    // addCelestialBody(std::make_unique<Saturn>());

    // // Uranus
    // addCelestialBody(std::make_unique<Uranus>());

    // // Neptune
    // addCelestialBody(std::make_unique<Neptune>());

    // Initialize lighting mode for all celestial bodies
    for (size_t i = 0; i < celestialBodies.size(); ++i)
    {
        celestialBodies[i]->setLightingMode(enableDynamicLighting);
    }
    
    // Create planet-specific lighting system
    createPlanetLightsSystem();

    return true;
}

void Scene::run()
{
    std::cout << "=== Modern Solar System - Modular Architecture ===" << std::endl;
    std::cout << "Classes are now properly separated!" << std::endl;
    std::cout << "NEW FEATURE: Distance-based Planet Lighting with Debug Visualization!" << std::endl;
    std::cout << "NEW FEATURE: Dynamic Moon Shadow System!" << std::endl;
    std::cout << "- Each planet has its own light source positioned at distance" << std::endl;
    std::cout << "- Light intensity varies based on distance from sun (inverse square law)" << std::endl;
    std::cout << "- Moon casts realistic shadows on Earth when it passes between light and planet" << std::endl;
    std::cout << "- Lights follow planetary orbital motion dynamically" << std::endl;
    std::cout << "\nControls:" << std::endl;
    std::cout << "- WASD: Move camera" << std::endl;
    std::cout << "- Mouse: Look around" << std::endl;
    std::cout << "- L: Toggle light visualizer (IMPORTANT: Shows planet light positions!)" << std::endl;
    std::cout << "- P: Toggle lighting & shadows (combined on/off)" << std::endl;
    std::cout << "- ESC: Exit" << std::endl;
    std::cout << "===========================================" << std::endl;

    string satellitePath = "assets/models/satellite.obj";
    
    // Create satellite shader using the modern Shader class
    satelliteShader = std::unique_ptr<Shader>(Shader::fromFiles("shaders/object.vert", "shaders/object.frag"));

    int satelliteVertices;
    GLuint satelliteVAO = setupModelVBO(satellitePath, satelliteVertices);

    glm::mat4 satelliteModelMatrix = glm::mat4(1.0f);
    satelliteModelMatrix = glm::translate(satelliteModelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
    satelliteModelMatrix = glm::scale(satelliteModelMatrix, glm::vec3(0.01f));

    int activeVAOVertices = satelliteVertices;
    GLuint activeVAO = satelliteVAO;

    // Spinning cube at camera position
    float spinningAngle = 0.0f;
    float lastFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {

        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput();

        // Update celestial bodies (orbital motion, rotation)
        for (size_t i = 0; i < celestialBodies.size(); ++i)
        {
            celestialBodies[i]->update(deltaTime);
        }
        
        // Update planet-specific lights to follow their planets
        updatePlanetLights();
        
        // Update lighting system only if enabled
        if (enableDynamicLighting)
        {
            lightManager.update(deltaTime);
            // Update planet lights to follow orbital motion and adjust intensity
            updatePlanetLights();
        }

        // First pass: Render shadow map
        if (enableShadows && enableDynamicLighting) {
            renderShadowPass();
        }

        // Second pass: Render scene with shadows
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float aspectRatio = (float)windowWidth / (float)windowHeight;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
        glm::mat4 view = camera.getViewMatrix();

        // Calculate light direction for consistent shadow mapping
        glm::vec3 lightDirection = calculateSunToEarthDirection();

        // Render all objects with shadow support
        for (size_t i = 0; i < celestialBodies.size(); ++i)
        {
            if (enableDynamicLighting) {
                celestialBodies[i]->renderWithShadows(view, projection, lightManager, camera.getPosition(), shadowMap.get(), enableShadows, lightDirection);
            } else {
                celestialBodies[i]->render(view, projection);
            }
        }
        for (size_t i = 0; i < objects.size(); ++i)
        {
            objects[i]->render(view, projection);
        }
        
        // Render light indicators if enabled and lighting is on
        if (showLightVisualizer && enableDynamicLighting)
        {
            lightVisualizer->renderLights(view, projection, lightManager);
        }

        float spinSpeed = 100.0f;
        if (isSpinning)
        {
            spinningAngle += 60.0f * dt;
        }

        // Create satellite transformation matrix with proper spinning control
        mat4 satelliteModelMatrix =
            glm::translate(mat4(1.0f), vec3(-70.0f, 0.0f, -20.0f)) *
            glm::rotate(mat4(1.0f), radians(spinningAngle), vec3(0.0f, 1.0f, 0.0f)) * // <-- Spin controlled by isSpinning
            glm::rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)) *        // <-- Tilt it on its side
            glm::scale(mat4(1.0f), vec3(2.0f));

        glm::mat4 projectionMatrix = glm::perspective(
            glm::radians(70.0f),
            800.0f / 600.0f,
            0.1f,
            1000.0f);

        // Use the modern Shader class for satellite rendering
        satelliteShader->use();
        satelliteShader->setMat4("projectionMatrix", projectionMatrix);
        satelliteShader->setMat4("worldMatrix", satelliteModelMatrix);
        satelliteShader->setMat4("viewMatrix", view);

        glBindVertexArray(satelliteVAO);
        glDrawArrays(GL_TRIANGLES, 0, satelliteVertices);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Scene::cleanup()
{
    objects.clear();
    glfwTerminate();
}

void Scene::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(0, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(1, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(2, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(3, deltaTime);
    
    // Toggle light visualizer with L key
    static bool lKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !lKeyPressed)
    {
        showLightVisualizer = !showLightVisualizer;
        lKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE)
    {
        lKeyPressed = false;
    }
    
    // Toggle dynamic lighting and shadows with P key (combined)
    static bool pKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pKeyPressed)
    {
        enableDynamicLighting = !enableDynamicLighting;
        enableShadows = enableDynamicLighting; // Shadows follow lighting state
        
        // Update all celestial bodies to use appropriate shaders
        for (size_t i = 0; i < celestialBodies.size(); ++i)
        {
            celestialBodies[i]->setLightingMode(enableDynamicLighting);
        }
        
        std::cout << "Dynamic Lighting & Shadows " << (enableDynamicLighting ? "enabled" : "disabled") << std::endl;
        pKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
    {
        pKeyPressed = false;
    }
    static bool spacePressedLastFrame = false;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (!spacePressedLastFrame)
        {
            isSpinning = !isSpinning;
            spacePressedLastFrame = true;
        }
    }
    else
    {
        spacePressedLastFrame = false;
    }
}

void Scene::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (instance->firstMouse)
    {
        instance->lastX = xpos;
        instance->lastY = ypos;
        instance->firstMouse = false;
    }

    float xoffset = xpos - instance->lastX;
    float yoffset = instance->lastY - ypos;
    instance->lastX = xpos;
    instance->lastY = ypos;

    instance->camera.processMouseMovement(xoffset, yoffset);
}

void Scene::addCelestialBody(std::unique_ptr<CelestialBody> body)
{
    celestialBodies.push_back(std::move(body));
}

void Scene::createPlanetLightsSystem()
{
    // Clear existing planet light indices
    planetLightIndices.clear();
    
    // Get sun position (assuming first celestial body is the sun)
    glm::vec3 sunPosition(0.0f, 0.0f, 0.0f);
    if (!celestialBodies.empty()) {
        sunPosition = celestialBodies[0]->getPosition();
    }
    
    std::cout << "\n=== Creating Planet Lighting System ===" << std::endl;
    std::cout << "Sun position: (" << sunPosition.x << ", " << sunPosition.y << ", " << sunPosition.z << ")" << std::endl;
    
    // Create individual lights for each planet (skip sun at index 0 and moons)
    for (size_t i = 1; i < celestialBodies.size(); ++i) {
        // Skip moons - we only want lights for planets
        // Check if this celestial body is a Moon by attempting dynamic cast
        if (dynamic_cast<Moon*>(celestialBodies[i].get()) != nullptr) {
            std::cout << "Skipping light creation for Moon at index " << i << std::endl;
            continue;
        }
        
        glm::vec3 planetPosition = celestialBodies[i]->getPosition();
        
        // Calculate distance from sun for intensity
        float distanceFromSun = glm::length(planetPosition - sunPosition);
        float baseSunDistance = 50.0f;
        float intensityFactor = (baseSunDistance * baseSunDistance) / (distanceFromSun * distanceFromSun);
        intensityFactor = glm::clamp(intensityFactor, 0.1f, 2.0f);
        
        // Create a warm sunlight color with slight variations for each planet
        glm::vec3 lightColor;
        switch (i % 4) {
            case 1: lightColor = glm::vec3(1.0f, 0.95f, 0.8f); break;  // Warm white
            case 2: lightColor = glm::vec3(1.0f, 0.9f, 0.7f); break;   // Slightly warmer
            case 3: lightColor = glm::vec3(0.95f, 0.9f, 0.85f); break; // Cooler white
            default: lightColor = glm::vec3(1.0f, 0.92f, 0.75f); break; // Golden white
        }
        
        // Add planet-specific light
        size_t lightIndex = lightManager.getLightCount();
        lightManager.addPlanetLight(sunPosition, planetPosition, lightColor);
        planetLightIndices.push_back(lightIndex);
        
        std::cout << "Planet " << i << ": Distance from sun = " << distanceFromSun 
                  << ", Intensity factor = " << intensityFactor 
                  << ", Position = (" << planetPosition.x << ", " << planetPosition.y << ", " << planetPosition.z << ")" << std::endl;
    }
    
    std::cout << "Created " << planetLightIndices.size() << " planet lights for debugging visualization." << std::endl;
    std::cout << "Use 'L' key to toggle light visualizer to see light positions." << std::endl;
    std::cout << "========================================\n" << std::endl;
}

void Scene::updatePlanetLights()
{
    // Get current sun position
    glm::vec3 sunPosition(0.0f, 0.0f, 0.0f);
    if (!celestialBodies.empty()) {
        sunPosition = celestialBodies[0]->getPosition();
    }
    
    // Update each planet light to follow its planet's orbital motion
    // We need to track which light corresponds to which planet since we skip moons
    size_t lightIndex = 0;
    for (size_t i = 1; i < celestialBodies.size(); ++i) {
        // Skip moons - they don't have lights
        if (dynamic_cast<Moon*>(celestialBodies[i].get()) != nullptr) {
            continue;
        }
        
        // This is a planet, update its corresponding light
        if (lightIndex < planetLightIndices.size()) {
            glm::vec3 planetPosition = celestialBodies[i]->getPosition();
            size_t actualLightIndex = planetLightIndices[lightIndex];
            lightManager.updatePlanetLight(actualLightIndex, sunPosition, planetPosition);
            lightIndex++;
        }
    }
}

void Scene::renderShadowPass() {
    if (!enableShadows) return;
    
    // Calculate light space matrix from sun to earth direction
    glm::vec3 lightDirection = calculateSunToEarthDirection();
    glm::vec3 sceneCenter(0.0f, 0.0f, 0.0f);
    glm::mat4 lightSpaceMatrix = shadowMap->calculateLightSpaceMatrix(lightDirection, sceneCenter, 150.0f);
    
    // Begin shadow pass
    shadowMap->beginShadowPass(lightSpaceMatrix);
    
    // Render all celestial bodies to shadow map (moon will cast shadows)
    for (size_t i = 0; i < celestialBodies.size(); ++i) {
        // Only render moon and other small objects that can cast shadows
        if (dynamic_cast<Moon*>(celestialBodies[i].get()) != nullptr) {
            // Render moon to shadow map using shadow shader
            shadowMap->getShadowShader()->use();
            shadowMap->getShadowShader()->setMat4("model", celestialBodies[i]->getModelMatrix());
            shadowMap->getShadowShader()->setMat4("lightSpaceMatrix", lightSpaceMatrix);
            
            // Render moon geometry
            glBindVertexArray(celestialBodies[i]->getVAO());
            glDrawElements(GL_TRIANGLES, celestialBodies[i]->getIndexCount(), GL_UNSIGNED_INT, 0);
        }
    }
    
    shadowMap->endShadowPass();
    
    // Reset viewport for main render pass
    glViewport(0, 0, windowWidth, windowHeight);
}

glm::vec3 Scene::calculateSunToEarthDirection() {
    if (celestialBodies.size() < 2) return glm::vec3(0.0f, 0.0f, 1.0f);
    
    glm::vec3 sunPos = celestialBodies[0]->getPosition(); // Sun
    glm::vec3 earthPos = celestialBodies[1]->getPosition(); // Earth (first planet)
    
    return glm::normalize(earthPos - sunPos);
}

void Scene::setWindowSize(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    if (window)
    {
        glfwSetWindowSize(window, width, height);
    }
}

void Scene::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    // Update viewport
    glViewport(0, 0, width, height);

    // Update stored window dimensions
    if (instance)
    {
        instance->windowWidth = width;
        instance->windowHeight = height;
    }
}
