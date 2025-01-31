#include "Interface.h"

Interface::Interface(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer(renderer), buttonHovered(false) {
    // Pasek u góry (ca³a szerokoœæ ekranu)
    topBar = { 0, 0, screenWidth, 50 };

    // Przyciski na pasku
    pauseButton = { 10, 10, 100, 30 }; // Prostok¹t przycisku (10 px od lewej, 10 px od góry)
}

void Interface::draw() {
    // Rysowanie paska
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Bia³y kolor
    SDL_RenderFillRect(renderer, &topBar);

    // Rysowanie przycisku
    if (buttonHovered) {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Jasnoszary, gdy najechano myszk¹
    }
    else {
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); // Szary domyœlny kolor
    }
    SDL_RenderFillRect(renderer, &pauseButton);

    // Rysowanie tekstu na przycisku (zastêpczy prostok¹t jako tekst)
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
            // Klikniêcie przycisku - zmiana stanu pauzy
            isPaused = !isPaused;
        }
    }
}
