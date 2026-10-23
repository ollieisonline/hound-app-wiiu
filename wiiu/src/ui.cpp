#include "ui.h"
#include <iostream>

UIManager::UIManager(SDL_Renderer* renderer) : renderer(renderer), font(nullptr), focusedIndex(-1) {
    // Load default font
    font = TTF_OpenFont("assets/fonts/DejaVuSans.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
}

UIManager::~UIManager() {
    if (font) {
        TTF_CloseFont(font);
    }
}

void UIManager::init() {
    // Initialize UI
}

void UIManager::addButton(int x, int y, int w, int h, const std::string& text, std::function<void()> onClick) {
    UIElement element;
    element.type = UIElement::BUTTON;
    element.rect = {x, y, w, h};
    element.text = text;
    element.textColor = {255, 255, 255, 255};
    element.onClick = onClick;
    element.focused = false;
    elements.push_back(element);
    
    if (focusedIndex == -1) {
        focusedIndex = 0;
        elements[0].focused = true;
    }
}

void UIManager::addText(int x, int y, const std::string& text, SDL_Color color) {
    UIElement element;
    element.type = UIElement::TEXT;
    element.rect = {x, y, 0, 0};
    element.text = text;
    element.textColor = color;
    elements.push_back(element);
}

void UIManager::addImage(int x, int y, int w, int h, SDL_Texture* texture) {
    UIElement element;
    element.type = UIElement::IMAGE;
    element.rect = {x, y, w, h};
    element.texture = texture;
    elements.push_back(element);
}

void UIManager::setFocusedElement(int index) {
    if (index >= 0 && index < elements.size()) {
        if (focusedIndex >= 0 && focusedIndex < elements.size()) {
            elements[focusedIndex].focused = false;
        }
        focusedIndex = index;
        elements[index].focused = true;
    }
}

void UIManager::nextElement() {
    if (elements.empty()) return;
    
    int nextIndex = focusedIndex + 1;
    if (nextIndex >= elements.size()) {
        nextIndex = 0;
    }
    setFocusedElement(nextIndex);
}

void UIManager::prevElement() {
    if (elements.empty()) return;
    
    int prevIndex = focusedIndex - 1;
    if (prevIndex < 0) {
        prevIndex = elements.size() - 1;
    }
    setFocusedElement(prevIndex);
}

void UIManager::activateElement() {
    if (focusedIndex >= 0 && focusedIndex < elements.size()) {
        if (elements[focusedIndex].type == UIElement::BUTTON && elements[focusedIndex].onClick) {
            elements[focusedIndex].onClick();
        }
    }
}

void UIManager::handleEvent(const SDL_Event& event) {
    // Handle UI-specific events
    switch (event.type) {
        case SDL_JOYBUTTONDOWN:
            if (event.jbutton.button == 0) { // A button
                activateElement();
            }
            break;
    }
}

void UIManager::render() {
    for (const auto& element : elements) {
        switch (element.type) {
            case UIElement::BUTTON: {
                SDL_Color bgColor = element.focused ? 
                    SDL_Color{100, 100, 200, 255} : 
                    SDL_Color{50, 50, 100, 255};
                
                SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
                SDL_RenderFillRect(renderer, &element.rect);
                
                // Render button border
                SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
                SDL_Rect border = element.rect;
                SDL_RenderDrawRect(renderer, &border);
                
                // Render text
                if (font && !element.text.empty()) {
                    SDL_Surface* textSurface = TTF_RenderText_Solid(font, element.text.c_str(), element.textColor);
                    if (textSurface) {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                        if (textTexture) {
                            SDL_Rect textRect = {
                                element.rect.x + (element.rect.w - textSurface->w) / 2,
                                element.rect.y + (element.rect.h - textSurface->h) / 2,
                                textSurface->w,
                                textSurface->h
                            };
                            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                            SDL_DestroyTexture(textTexture);
                        }
                        SDL_FreeSurface(textSurface);
                    }
                }
                break;
            }
            case UIElement::TEXT: {
                if (font) {
                    SDL_Surface* textSurface = TTF_RenderText_Solid(font, element.text.c_str(), element.textColor);
                    if (textSurface) {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                        if (textTexture) {
                            SDL_Rect textRect = {
                                element.rect.x,
                                element.rect.y,
                                textSurface->w,
                                textSurface->h
                            };
                            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                            SDL_DestroyTexture(textTexture);
                        }
                        SDL_FreeSurface(textSurface);
                    }
                }
                break;
            }
            case UIElement::IMAGE: {
                if (element.texture) {
                    SDL_RenderCopy(renderer, element.texture, nullptr, &element.rect);
                }
                break;
            }
        }
    }
}
