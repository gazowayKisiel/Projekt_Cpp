#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL.h>

class Interface {
public:
    Interface(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    void draw();
    void handleEvent(const SDL_Event& e, bool& isPaused);

private:
    SDL_Renderer* renderer;
    SDL_Rect topBar;       // Pasek u góry
    SDL_Rect pauseButton;  // Prostok¹t przycisku
    bool buttonHovered;    // Czy kursor jest nad przyciskiem
};

#endif
