#pragma once

#include "CelestialBody.h"

class Sun : public CelestialBody {
public:
    Sun();
    
    // Override update to ensure Sun never moves from center
    void update(float deltaTime) override;
    
protected:
    std::string getTexturePath() override { return "assets/sun.jpg"; }
    std::string getVertexShaderPath() override; // Simple vertex shader without lighting
    std::string getFragmentShaderPath() override; // Custom shader for glowing effect
};
