#include "Interface.h"

Interface::Interface(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer(renderer), buttonHovered(false) {
    // Pasek u g�ry (ca�a szeroko�� ekranu)
    topBar = { 0, 0, screenWidth, 50 };

    // Przyciski na pasku
    pauseButton = { 10, 10, 100, 30 }; // Prostok�t przycisku (10 px od lewej, 10 px od g�ry)
}

void Interface::draw() {
    // Rysowanie paska
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Bia�y kolor
    SDL_RenderFillRect(renderer, &topBar);

    // Rysowanie przycisku
    if (buttonHovered) {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Jasnoszary, gdy najechano myszk�
    }
    else {
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); // Szary domy�lny kolor
    }
    SDL_RenderFillRect(renderer, &pauseButton);

    // Rysowanie tekstu na przycisku (zast�pczy prostok�t jako tekst)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Czarny tekst
    SDL_RenderDrawLine(renderer, pauseButton.x + 10, pauseButton.y + 10, pauseButton.x + 90, pauseButton.y + 10);
    SDL_RenderDrawLine(renderer, pauseButton.x + 10, pauseButton.y + 20, pauseButton.x + 90, pauseButton.y + 20);
}

void Interface::handleEvent(const SDL_Event& e, bool& isPaused) {
    int mouseX, mouseY;
    if (e.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&mouseX, &mouseY);
        // Sprawdzenie, czy myszka jest nad przyciskiem
        buttonHovered = SDL_PointInRect(&SDL_Point{ mouseX, mouseY }, &pauseButton);
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        SDL_GetMouseState(&mouseX, &mouseY);
        if (SDL_PointInRect(&SDL_Point{ mouseX, mouseY }, &pauseButton)) {
            // Klikni�cie przycisku - zmiana stanu pauzy
            isPaused = !isPaused;
        }
    }
}
