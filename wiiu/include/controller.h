#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <functional>

class ControllerManager {
public:
    ControllerManager();
    ~ControllerManager();
    
    void init();
    void update();
    void handleEvent(const SDL_Event& event);
    
    // Button states
    bool isButtonPressed(SDL_GameControllerButton button);
    bool isButtonJustPressed(SDL_GameControllerButton button);
    bool isButtonJustReleased(SDL_GameControllerButton button);
    
    // Axis states
    float getAxisValue(SDL_GameControllerAxis axis);
    
    // Controller connection
    bool isControllerConnected();
    int getControllerCount();
    
    // Wii U specific buttons (mapped to standard controller buttons)
    enum WiiUButton {
        WIIU_A = SDL_CONTROLLER_BUTTON_A,
        WIIU_B = SDL_CONTROLLER_BUTTON_B,
        WIIU_X = SDL_CONTROLLER_BUTTON_X,
        WIIU_Y = SDL_CONTROLLER_BUTTON_Y,
        WIIU_DPAD_UP = SDL_CONTROLLER_BUTTON_DPAD_UP,
        WIIU_DPAD_DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
        WIIU_DPAD_LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        WIIU_DPAD_RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        WIIU_L = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        WIIU_R = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
        WIIU_ZL = SDL_CONTROLLER_BUTTON_LEFTSTICK,
        WIIU_ZR = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
        WIIU_PLUS = SDL_CONTROLLER_BUTTON_START,
        WIIU_MINUS = SDL_CONTROLLER_BUTTON_BACK,
        WIIU_HOME = SDL_CONTROLLER_BUTTON_GUIDE
    };
    
    // Wii U specific axes
    enum WiiUAxis {
        WIIU_LEFT_STICK_X = SDL_CONTROLLER_AXIS_LEFTX,
        WIIU_LEFT_STICK_Y = SDL_CONTROLLER_AXIS_LEFTY,
        WIIU_RIGHT_STICK_X = SDL_CONTROLLER_AXIS_RIGHTX,
        WIIU_RIGHT_STICK_Y = SDL_CONTROLLER_AXIS_RIGHTY,
        WIIU_L_TRIGGER = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
        WIIU_R_TRIGGER = SDL_CONTROLLER_AXIS_TRIGGERRIGHT
    };
    
private:
    SDL_GameController* controller;
    std::map<SDL_GameControllerButton, bool> buttonStates;
    std::map<SDL_GameControllerButton, bool> buttonPrevStates;
    std::map<SDL_GameControllerAxis, float> axisStates;
    
    void openController(int index);
    void closeController();
};
