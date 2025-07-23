#pragma once

#include "CelestialBody.h"

class Sun : public CelestialBody {
public:
    Sun();
    
protected:
    std::string getTexturePath() override { return "assets/sun.jpg"; }
    std::string getFragmentShader() override; // Custom shader for glowing effect
};
