#include "app.h"
#include "ui.h"
#include "controller.h"
#include "hound_client.h"
#include <iostream>

App::App() : window(nullptr), renderer(nullptr), running(false), 
    uiManager(nullptr), controllerManager(nullptr), houndClient(nullptr) {}

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
    
    // Initialize managers
    uiManager = new UIManager(renderer);
    uiManager->init();
    
    controllerManager = new ControllerManager();
    controllerManager->init();
    
    houndClient = new HoundClient();
    
    if (!loadAssets()) {
        std::cerr << "Failed to load assets" << std::endl;
        return false;
    }
    
    currentState = AppState::MAIN_MENU;
    setupMainMenu();
    
    running = true;
    return true;
}

bool App::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NET | SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "SDL could not initialize: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize: " << IMG_GetError() << std::endl;
        return false;
    }
    
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize: " << TTF_GetError() << std::endl;
        return false;
    }
    
    if (SDLNet_Init() == -1) {
        std::cerr << "SDL_net could not initialize: " << SDLNet_GetError() << std::endl;
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

bool App::loadAssets() {
    // TODO: Load fonts, textures, etc.
    return true;
}

void App::setupMainMenu() {
    // Clear UI by recreating the UI manager
    if (uiManager) {
        delete uiManager;
    }
    uiManager = new UIManager(renderer);
    uiManager->init();
    
    // Add main menu buttons
    uiManager->addText(50, 50, "Hound App - Wii U", {255, 255, 255, 255});
    uiManager->addText(50, 100, "Main Menu", {200, 200, 200, 255});
    
    uiManager->addButton(50, 150, 300, 60, "Media Library", [this]() {
        currentState = AppState::MEDIA_LIST;
        setupMediaList();
    });
    
    uiManager->addButton(50, 220, 300, 60, "Collections", [this]() {
        // Show collections
    });
    
    uiManager->addButton(50, 290, 300, 60, "Search", [this]() {
        // Show search
    });
    
    uiManager->addButton(50, 360, 300, 60, "Settings", [this]() {
        // Show settings
    });
    
    uiManager->addButton(50, 430, 300, 60, "Exit", [this]() {
        running = false;
    });
}

void App::setupMediaList() {
    // Clear UI
    if (uiManager) {
        delete uiManager;
    }
    uiManager = new UIManager(renderer);
    uiManager->init();
    
    // Add header
    uiManager->addText(50, 50, "Media Library", {255, 255, 255, 255});
    uiManager->addText(50, 100, "Select a media item", {200, 200, 200, 255});
    
    // Add media items (placeholder)
    for (int i = 0; i < 5; i++) {
        uiManager->addButton(50, 150 + i * 70, 400, 60, 
            "Media Item " + std::to_string(i), [this, i]() {
                // Show media details
                currentState = AppState::MEDIA_DETAILS;
            });
    }
    
    // Back button
    uiManager->addButton(50, 600, 200, 50, "Back", [this]() {
        currentState = AppState::MAIN_MENU;
        setupMainMenu();
    });
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
                // Handle controller button presses
                if (event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
                    uiManager->prevElement();
                } else if (event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
                    uiManager->nextElement();
                } else if (event.jbutton.button == SDL_CONTROLLER_BUTTON_A) {
                    uiManager->activateElement();
                } else if (event.jbutton.button == SDL_CONTROLLER_BUTTON_B) {
                    // Back button
                    if (currentState == AppState::MEDIA_LIST) {
                        currentState = AppState::MAIN_MENU;
                        setupMainMenu();
                    } else if (currentState == AppState::MEDIA_DETAILS) {
                        currentState = AppState::MEDIA_LIST;
                        setupMediaList();
                    }
                }
                break;
            case SDL_JOYAXISMOTION:
                // Handle joystick movement
                break;
        }
        
        // Pass event to managers
        if (uiManager) {
            uiManager->handleEvent(event);
        }
        if (controllerManager) {
            controllerManager->handleEvent(event);
        }
    }
    
    // Update controller manager
    if (controllerManager) {
        controllerManager->update();
    }
}

void App::update() {
    // Update game state
    switch (currentState) {
        case AppState::MAIN_MENU:
            // Update main menu
            break;
        case AppState::MEDIA_LIST:
            // Update media list
            break;
        case AppState::MEDIA_DETAILS:
            // Update media details
            break;
        case AppState::PLAYING:
            // Update playback
            break;
    }
}

void App::render() {
    SDL_SetRenderDrawColor(renderer, 0x18, 0x0B, 0x6F, 0xFF);
    SDL_RenderClear(renderer);
    
    // Render UI
    if (uiManager) {
        uiManager->render();
    }
    
    // Render state-specific elements
    switch (currentState) {
        case AppState::MAIN_MENU:
            renderMainMenu();
            break;
        case AppState::MEDIA_LIST:
            renderMediaList();
            break;
        case AppState::MEDIA_DETAILS:
            renderMediaDetails();
            break;
        case AppState::PLAYING:
            renderPlaying();
            break;
    }
    
    SDL_RenderPresent(renderer);
}

void App::renderMainMenu() {
    // Additional main menu rendering
}

void App::renderMediaList() {
    // Additional media list rendering
}

void App::renderMediaDetails() {
    // Additional media details rendering
}

void App::renderPlaying() {
    // Video playback rendering
}

void App::cleanup() {
    if (uiManager) {
        delete uiManager;
        uiManager = nullptr;
    }
    
    if (controllerManager) {
        delete controllerManager;
        controllerManager = nullptr;
    }
    
    if (houndClient) {
        delete houndClient;
        houndClient = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    SDLNet_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
