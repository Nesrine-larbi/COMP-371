#include "../include/Planet.h"

// Base Planet implementation
Planet::Planet(const std::string &texturePath, float planetScale)
    : texturePath_(texturePath)
{
    setScale(glm::vec3(planetScale));

    // Initialize with common sphere generation and texture loading
    generateSphere();
    setupShaders(getVertexShaderPath(), getFragmentShaderPath());
    loadTexture(getTexturePath());
}

// Specific planet implementations with realistic scales and orbital properties
Earth::Earth() : Planet("assets/earth.jpg", 4.0f)
{
    setOrbitalRadius(100.0f);
    setOrbitalSpeed(0.3f);
    setRotationSpeed(0.5f);
}
// Moon implementation as a planet (child of Earth)
Moon::Moon(CelestialBody *parent, float orbitalRadius, float orbitalSpeed, float rotationSpeed, float moonScale) : Planet("assets/moon.png", moonScale)
{
    // setOrbitalRadius(10.0f);
    // setOrbitalSpeed(1.5f);
    // setRotationSpeed(1.0f);

    setOrbitalRadius(orbitalRadius);
    setOrbitalSpeed(orbitalSpeed);
    setRotationSpeed(rotationSpeed);
    setParent(parent);
}

Mars::Mars() : Planet("assets/mars.jpg", 3.0f)
{
    setOrbitalRadius(150.0f);
    setOrbitalSpeed(0.15f);
    setRotationSpeed(1.9f);
}

Venus::Venus() : Planet("assets/venus.jpg", 3.8f)
{
    setOrbitalRadius(80.0f);
    setOrbitalSpeed(0.5f);
    setRotationSpeed(-0.5f); // Venus rotates backwards
}

Jupiter::Jupiter() : Planet("assets/jupiter.jpg", 12.0f)
{
    setOrbitalRadius(200.0f);
    setOrbitalSpeed(0.08f);
    setRotationSpeed(4.0f); // Fast rotation
}

Saturn::Saturn() : Planet("assets/saturn.jpg", 10.0f)
{
    setOrbitalRadius(250.0f);
    setOrbitalSpeed(0.06f);
    setRotationSpeed(3.5f);
}

Mercury::Mercury() : Planet("assets/mercury.jpg", 2.0f)
{
    setOrbitalRadius(50.0f);
    setOrbitalSpeed(0.8f);
    setRotationSpeed(0.3f); // Slow rotation
}

Uranus::Uranus() : Planet("assets/uranus.jpg", 8.0f)
{
    setOrbitalRadius(300.0f);
    setOrbitalSpeed(0.04f);
    setRotationSpeed(1.5f);
}

Neptune::Neptune() : Planet("assets/neptune.jpg", 7.5f)
{
    setOrbitalRadius(350.0f);
    setOrbitalSpeed(0.03f);
    setRotationSpeed(1.8f);
}

Pluto::Pluto() : Planet("assets/pluto.jpg", 1.5f)
{
    setOrbitalRadius(400.0f);
    setOrbitalSpeed(0.02f);
    setRotationSpeed(0.8f);
}
