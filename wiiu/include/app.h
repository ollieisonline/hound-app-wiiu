#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <string>
#include <vector>
#include <memory>

// Forward declarations
class UIManager;
class ControllerManager;
class HoundClient;

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
    bool loadAssets();
    
    void handleEvents();
    void update();
    void render();
    
    void setupMainMenu();
    void setupMediaList();
    void renderMainMenu();
    void renderMediaList();
    void renderMediaDetails();
    void renderPlaying();
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    
    // Screen dimensions
    static constexpr int SCREEN_WIDTH = 1280;
    static constexpr int SCREEN_HEIGHT = 720;
    
    // Managers
    UIManager* uiManager;
    ControllerManager* controllerManager;
    HoundClient* houndClient;
    
    // App state
    enum class AppState { MAIN_MENU, MEDIA_LIST, MEDIA_DETAILS, PLAYING };
    AppState currentState;
};
