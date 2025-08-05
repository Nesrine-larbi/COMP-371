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

using namespace glm;
using namespace std;

const char *getVertexShaderSource()
{
    return "#version 330 core\n"
           "layout (location = 0) in vec3 aPos;"
           "layout (location = 1) in vec3 aNormal;"
           ""
           "out vec3 vertexNormal;"
           ""
           "uniform mat4 worldMatrix;"
           "uniform mat4 viewMatrix = mat4(1.0);"
           "uniform mat4 projectionMatrix = mat4(1.0);"
           ""
           "void main()"
           "{"
           "   "
           "   vertexNormal = aNormal;"
           "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
           "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
           "}";
}

const char *getFragmentShaderSource()
{
    return "#version 330 core\n"
           "in vec3 vertexNormal;"
           "out vec4 FragColor;"
           "void main()"
           "{"
           "   FragColor = vec4(0.5f*vertexNormal+vec3(0.5f), 1.0f);"
           "}";
}

int compileAndLinkShaders(const char *vertexShaderSource, const char *fragmentShaderSource)
{
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

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

    // Saturn
    addCelestialBody(std::make_unique<Saturn>());

    // Uranus
    addCelestialBody(std::make_unique<Uranus>());

    // Neptune
    addCelestialBody(std::make_unique<Neptune>());

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

    string satellitePath = "assets/models/satellite.obj";
    int whiteShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());

    int satelliteVertices;
    GLuint satelliteVAO = setupModelEBO(satellitePath, satelliteVertices);

    while (!glfwWindowShouldClose(window))
    {
        glUseProgram(whiteShaderProgram);

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

        mat4 satelliteModelMatrix =
            glm::translate(mat4(1.0f), vec3(2.0f, 0.0f, -5.0f)) *
            glm::scale(mat4(1.0f), vec3(25.0f));

        mat4 projectionMatrix = glm::perspective(70.0f,
                                                 800.0f / 600.0f,
                                                 0.01f, 100.0f);

        setProjectionMatrix(whiteShaderProgram, projectionMatrix);
        setViewMatrix(whiteShaderProgram, view);
        setWorldMatrix(whiteShaderProgram, satelliteModelMatrix);

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
