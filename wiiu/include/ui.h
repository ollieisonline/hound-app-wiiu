#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <functional>

class UIManager {
public:
    UIManager(SDL_Renderer* renderer);
    ~UIManager();
    
    void init();
    void render();
    void handleEvent(const SDL_Event& event);
    
    // UI Elements
    void addButton(int x, int y, int w, int h, const std::string& text, std::function<void()> onClick);
    void addText(int x, int y, const std::string& text, SDL_Color color = {255, 255, 255, 255});
    void addImage(int x, int y, int w, int h, SDL_Texture* texture);
    
    void setFocusedElement(int index);
    void nextElement();
    void prevElement();
    void activateElement();
    
private:
    struct UIElement {
        enum Type { BUTTON, TEXT, IMAGE };
        Type type;
        SDL_Rect rect;
        std::string text;
        SDL_Color textColor;
        SDL_Texture* texture;
        std::function<void()> onClick;
        bool focused;
    };
    
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<UIElement> elements;
    int focusedIndex;
};
