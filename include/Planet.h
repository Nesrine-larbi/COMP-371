#pragma once

#include "CelestialBody.h"

// Specific implementation for planets
class Planet : public CelestialBody {
public:
    Planet(const std::string& texturePath, float planetScale = 1.0f);
    
protected:
    std::string getTexturePath() override { return texturePath_; }
    
private:
    std::string texturePath_;
};

// Earth with specific properties
class Earth : public Planet {
public:
    Earth();
};

class Moon : public Planet {
public:
    Moon(CelestialBody* parent);
};

// Mars with specific properties  
class Mars : public Planet {
public:
    Mars();
};

// Venus with specific properties
class Venus : public Planet {
public:
    Venus();
};

// Jupiter with specific properties
class Jupiter : public Planet {
public:
    Jupiter();
};

// Saturn with specific properties
class Saturn : public Planet {
public:
    Saturn();
};

// Mercury with specific properties
class Mercury : public Planet {
public:
    Mercury();
};

// Uranus with specific properties
class Uranus : public Planet {
public:
    Uranus();
};

// Neptune with specific properties
class Neptune : public Planet {
public:
    Neptune();
};

// Pluto with specific properties
class Pluto : public Planet {
public:
    Pluto();
};
