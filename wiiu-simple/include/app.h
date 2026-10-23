#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>

class App {
public:
    App();
    ~App();
    
    bool init();
    void run();
    void cleanup();
    
private:
    bool initSDL();
    bool initWindow();
    bool initRenderer();
    
    void handleEvents();
    void update();
    void render();
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    
    // Screen dimensions
    static constexpr int SCREEN_WIDTH = 1280;
    static constexpr int SCREEN_HEIGHT = 720;
    
    // App state
    enum class AppState { MAIN_MENU, MEDIA_LIST, PLAYING };
    AppState currentState;
    int selectedItem;
};
