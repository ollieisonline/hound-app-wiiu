#include "app.h"
#include <iostream>

App::App() : window(nullptr), renderer(nullptr), running(false), 
    currentState(AppState::MAIN_MENU), selectedItem(0) {}

App::~App() {
    cleanup();
}

bool App::init() {
    if (!initSDL()) {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return false;
    }
    
    if (!initWindow()) {
        std::cerr << "Failed to create window" << std::endl;
        return false;
    }
    
    if (!initRenderer()) {
        std::cerr << "Failed to create renderer" << std::endl;
        return false;
    }
    
    running = true;
    return true;
}

bool App::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "SDL could not initialize: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool App::initWindow() {
    window = SDL_CreateWindow(
        "Hound App - Wii U",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        std::cerr << "Window could not be created: " << SDL_GetError() << std::endl;
        return false;
    }
    
    return true;
}

bool App::initRenderer() {
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (!renderer) {
        std::cerr << "Renderer could not be created: " << SDL_GetError() << std::endl;
        return false;
    }
    
    SDL_SetRenderDrawColor(renderer, 0x18, 0x0B, 0x6F, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    return true;
}

void App::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                break;
            case SDL_JOYBUTTONDOWN:
                if (event.jbutton.button == SDL_CONTROLLER_BUTTON_A) {
                    // Select
                    if (currentState == AppState::MAIN_MENU) {
                        currentState = AppState::MEDIA_LIST;
                        selectedItem = 0;
                    }
                } else if (event.jbutton.button == SDL_CONTROLLER_BUTTON_B) {
                    // Back
                    if (currentState == AppState::MEDIA_LIST) {
                        currentState = AppState::MAIN_MENU;
                        selectedItem = 0;
                    }
                }
                break;
            case SDL_JOYAXISMOTION:
                // Handle joystick movement for navigation
                if (event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                    if (event.jaxis.value < -20000) {
                        // Up
                        selectedItem = (selectedItem - 1 + 5) % 5;
                    } else if (event.jaxis.value > 20000) {
                        // Down
                        selectedItem = (selectedItem + 1) % 5;
                    }
                }
                break;
        }
    }
}

void App::update() {
    // Update game state
}

void App::render() {
    SDL_SetRenderDrawColor(renderer, 0x18, 0x0B, 0x6F, 0xFF);
    SDL_RenderClear(renderer);
    
    // Draw UI based on state
    switch (currentState) {
        case AppState::MAIN_MENU:
            renderMainMenu();
            break;
        case AppState::MEDIA_LIST:
            renderMediaList();
            break;
        case AppState::PLAYING:
            renderPlaying();
            break;
    }
    
    SDL_RenderPresent(renderer);
}

void App::renderMainMenu() {
    // Draw title
    SDL_Rect titleRect = {50, 50, 400, 60};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &titleRect);
    
    // Draw menu items
    for (int i = 0; i < 3; i++) {
        SDL_Rect itemRect = {50, 150 + i * 80, 400, 60};
        if (i == selectedItem) {
            SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 100, 100, 150, 255);
        }
        SDL_RenderFillRect(renderer, &itemRect);
    }
}

void App::renderMediaList() {
    // Draw header
    SDL_Rect headerRect = {50, 50, 400, 60};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &headerRect);
    
    // Draw media items
    for (int i = 0; i < 5; i++) {
        SDL_Rect itemRect = {50, 150 + i * 80, 500, 60};
        if (i == selectedItem) {
            SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 100, 100, 150, 255);
        }
        SDL_RenderFillRect(renderer, &itemRect);
    }
}

void App::renderPlaying() {
    // Draw video area
    SDL_Rect videoRect = {50, 50, 1180, 620};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &videoRect);
    
    // Draw controls
    SDL_Rect controlsRect = {50, 700, 1180, 50};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &controlsRect);
}

void App::cleanup() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    SDL_Quit();
}
