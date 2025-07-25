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
