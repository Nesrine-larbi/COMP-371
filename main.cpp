//
// Modern Solar System - Clean Architecture Version  
// All classes properly separated for maintainability
//

#include "include/Scene.h"
#include <iostream>

int main() {
    Scene scene;
    
    if (!scene.initialize()) {
        std::cerr << "Failed to initialize scene" << std::endl;
        return -1;
    }
    
    scene.run();
    scene.cleanup();
    
    return 0;
}
