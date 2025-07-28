#define GLEW_STATIC 1
#include <GL/glew.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "../include/Scene.h"
#include "../include/Sun.h"
#include "../include/Skybox.h"
#include "../include/Planet.h"

Scene *Scene::instance = NULL;

Scene::Scene() : camera(glm::vec3(0.0f, 0.0f, 100.0f)), windowWidth(1920), windowHeight(800),
                 deltaTime(0.0f), lastFrame(0.0f), lastX(960), lastY(400), firstMouse(true)
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

    // Create scene objects
    objects.push_back(std::make_unique<Skybox>());
    addCelestialBody(std::make_unique<Sun>());

    // Test adding one planet at a time

    // Mercury
    addCelestialBody(std::make_unique<Mercury>());

    // Venus
    addCelestialBody(std::make_unique<Venus>());

    // Earth
    addCelestialBody(std::make_unique<Earth>());
    CelestialBody *earth = celestialBodies.back().get();
    addCelestialBody(std::make_unique<Moon>(earth));

    // Mars
    addCelestialBody(std::make_unique<Mars>());
    CelestialBody *mars = celestialBodies.back().get();
    addCelestialBody(std::make_unique<Moon>(mars, 9.4f, 1.0f, 1.0f, 1.0f));
    addCelestialBody(std::make_unique<Moon>(mars, 23.5f, 0.25f, 0.25f, 0.6f));

    // Jupiter
    addCelestialBody(std::make_unique<Jupiter>());

    return true;
}

void Scene::run()
{
    std::cout << "=== Modern Solar System - Modular Architecture ===" << std::endl;
    std::cout << "Classes are now properly separated!" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "- WASD: Move camera" << std::endl;
    std::cout << "- Mouse: Look around" << std::endl;
    std::cout << "- ESC: Exit" << std::endl;
    std::cout << "===========================================" << std::endl;

    while (!glfwWindowShouldClose(window))
    {
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

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float aspectRatio = (float)windowWidth / (float)windowHeight;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
        glm::mat4 view = camera.getViewMatrix();

        // Render all objects
        for (size_t i = 0; i < celestialBodies.size(); ++i)
        {
            celestialBodies[i]->render(view, projection);
        }
        for (size_t i = 0; i < objects.size(); ++i)
        {
            objects[i]->render(view, projection);
        }

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
