#include "controller.h"
#include <iostream>

ControllerManager::ControllerManager() : controller(nullptr) {
    // Initialize button states
    buttonStates[SDL_CONTROLLER_BUTTON_A] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_B] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_X] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_Y] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_DPAD_UP] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_DPAD_DOWN] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_LEFTSTICK] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_RIGHTSTICK] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_START] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_BACK] = false;
    buttonStates[SDL_CONTROLLER_BUTTON_GUIDE] = false;
}

ControllerManager::~ControllerManager() {
    closeController();
}

void ControllerManager::init() {
    // Initialize SDL game controller subsystem
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "SDL_INIT_GAMECONTROLLER failed: " << SDL_GetError() << std::endl;
        return;
    }
    
    // Open first available controller
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            openController(i);
            break;
        }
    }
    
    // If no game controller found, try to open any joystick
    if (!controller && SDL_NumJoysticks() > 0) {
        openController(0);
    }
}

void ControllerManager::openController(int index) {
    closeController();
    controller = SDL_GameControllerOpen(index);
    
    if (controller) {
        std::cout << "Opened controller: " << SDL_GameControllerName(controller) << std::endl;
    } else {
        std::cerr << "Failed to open controller: " << SDL_GetError() << std::endl;
    }
}

void ControllerManager::closeController() {
    if (controller) {
        SDL_GameControllerClose(controller);
        controller = nullptr;
    }
}

void ControllerManager::update() {
    // Store previous button states
    buttonPrevStates = buttonStates;
}

void ControllerManager::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_CONTROLLERDEVICEADDED:
            openController(event.cdevice.which);
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            if (event.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller))) {
                closeController();
            }
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            buttonStates[static_cast<SDL_GameControllerButton>(event.cbutton.button)] = true;
            break;
        case SDL_CONTROLLERBUTTONUP:
            buttonStates[static_cast<SDL_GameControllerButton>(event.cbutton.button)] = false;
            break;
        case SDL_CONTROLLERAXISMOTION:
            // Normalize axis value to [-1, 1]
            float value = static_cast<float>(event.caxis.value) / 32768.0f;
            axisStates[static_cast<SDL_GameControllerAxis>(event.caxis.axis)] = value;
            break;
    }
}

bool ControllerManager::isButtonPressed(SDL_GameControllerButton button) {
    return buttonStates[button];
}

bool ControllerManager::isButtonJustPressed(SDL_GameControllerButton button) {
    return buttonStates[button] && !buttonPrevStates[button];
}

bool ControllerManager::isButtonJustReleased(SDL_GameControllerButton button) {
    return !buttonStates[button] && buttonPrevStates[button];
}

float ControllerManager::getAxisValue(SDL_GameControllerAxis axis) {
    if (axisStates.find(axis) != axisStates.end()) {
        return axisStates[axis];
    }
    return 0.0f;
}

bool ControllerManager::isControllerConnected() {
    return controller != nullptr;
}

int ControllerManager::getControllerCount() {
    return SDL_NumJoysticks();
}
