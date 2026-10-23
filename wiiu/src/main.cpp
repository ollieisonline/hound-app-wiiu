#include "app.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Starting Hound App for Wii U..." << std::endl;
    
    App app;
    
    if (!app.init()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return 1;
    }
    
    app.run();
    
    return 0;
}
