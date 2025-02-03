#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
#include "Particle.h"


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf could not initialize! SDL_Error: %s", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Particle Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1400, 750, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Załaduj czcionkę
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24); // Zmień ścieżkę do czcionki
    if (font == nullptr) {
        SDL_Log("Failed to load font! SDL_ttf Error: %s", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<Particle> particles;

    // Tworzenie cząsteczek
    for (int i = 0; i < 19; i++) {
        double mass = 2 * (10.0 + static_cast<double>(std::rand() % 10));
        double radius = mass / 2;
        double x = std::rand() % (1400 - static_cast<int>(2 * radius)) + radius;
        double y = std::rand() % (700 - static_cast<int>(2 * radius)) + radius + 50;
        double vx = (std::rand() % 50 + 1) * (std::rand() % 2 == 0 ? 1 : -1);
        double vy = (std::rand() % 50 + 1) * (std::rand() % 2 == 0 ? 1 : -1);
        Uint8 r = std::rand() % 256;
        Uint8 g = std::rand() % 256;
        Uint8 b = std::rand() % 256;

        particles.emplace_back(x, y, vx, vy, radius, mass, r, g, b);
    }

    particles.emplace_back(100, 100, 0, 0, 40, 100000000000, 254, 254, 254);

    bool quit = false;
    bool isPaused = false;
    bool gravityEnabled = true;
    bool isDragging = false;
    bool isDraggingSlider = false;
    bool isDraggingGravitySlider = false;
    int selectedParticleIndex = -1;
    SDL_Event e;

    Uint32 lastTime = SDL_GetTicks();
    double timeScale = 2.0;
    double gravityStrength = 9.81;  // Początkowa siła grawitacji

    SDL_Rect pauseButton = { 20, 10, 100, 30 };
    SDL_Rect gravityButton = { 130, 10, 150, 30 };
    SDL_Rect speedSlider = { 300, 15, 200, 10 };
    SDL_Rect sliderHandle = { 300 + 100, 10, 10, 20 };
    SDL_Rect gravitySlider = { 700, 15, 200, 10 }; // Suwak dla grawitacji
    SDL_Rect gravitySliderHandle = { 700 + static_cast<int>(gravityStrength), 10, 10, 20 }; // Początkowa pozycja uchwytu

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - lastTime) / 1000.0 * timeScale;
        lastTime = currentTime;

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) { // Spacja - pauza/wznowienie
                    isPaused = !isPaused;
                    selectedParticleIndex = -1; // Odznaczenie cząsteczki
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Sprawdzanie kliknięcia na przycisk pauzy
                if (mouseX >= pauseButton.x && mouseX <= pauseButton.x + pauseButton.w &&
                    mouseY >= pauseButton.y && mouseY <= pauseButton.y + pauseButton.h) {
                    isPaused = !isPaused;
                    selectedParticleIndex = -1; // Odznaczenie cząsteczki
                }

                // Sprawdzanie kliknięcia na przycisk grawitacji
                if (mouseX >= gravityButton.x && mouseX <= gravityButton.x + gravityButton.w &&
                    mouseY >= gravityButton.y && mouseY <= gravityButton.y + gravityButton.h) {
                    gravityEnabled = !gravityEnabled;
                }

                // Sprawdzanie kliknięcia na slider dla siły grawitacji
                if (mouseX >= gravitySlider.x && mouseX <= gravitySlider.x + gravitySlider.w &&
                    mouseY >= gravitySlider.y && mouseY <= gravitySlider.y + gravitySlider.h) {
                    isDraggingGravitySlider = true;
                }

                // Sprawdzanie kliknięcia na slider
                if (mouseX >= sliderHandle.x && mouseX <= sliderHandle.x + sliderHandle.w &&
                    mouseY >= sliderHandle.y && mouseY <= sliderHandle.y + sliderHandle.h) {
                    isDraggingSlider = true;
                }

                // Sprawdzanie, czy kliknięto na cząsteczkę (pauza musi być aktywna)
                if (isPaused) {
                    selectedParticleIndex = -1;
                    for (size_t i = 0; i < particles.size(); i++) {
                        double dx = mouseX - particles[i].x;
                        double dy = mouseY - particles[i].y;
                        double distance = std::sqrt(dx * dx + dy * dy);

                        if (distance <= particles[i].radius) {
                            selectedParticleIndex = i;
                            isDragging = true;
                            break;
                        }
                    }
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                isDragging = false;
                isDraggingSlider = false;
                isDraggingGravitySlider = false;
            }
            else if (e.type == SDL_MOUSEMOTION) {
                if (isDraggingSlider) {
                    int newX = e.motion.x;
                    if (newX >= speedSlider.x && newX <= speedSlider.x + speedSlider.w) {
                        sliderHandle.x = newX;
                        timeScale = 0.5 + ((newX - speedSlider.x) / 200.0) * 9.5; // Maksymalna wartość to 10.0
                    }
                }

                // Zmiana siły grawitacji
                if (isDraggingGravitySlider) {
                    int newX = e.motion.x;
                    if (newX >= gravitySlider.x && newX <= gravitySlider.x + gravitySlider.w) {
                        gravitySliderHandle.x = newX;
                        gravityStrength = (newX - gravitySlider.x) / 200.0 * 50.0; // Maksymalna grawitacja to 50.0
                    }
                }

                if (isDragging && isPaused && selectedParticleIndex != -1) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    // Sprawdzanie kolizji i ograniczeń ekranu
                    bool validPosition = true;

                    for (size_t i = 0; i < particles.size(); i++) {
                        if (i != selectedParticleIndex) {
                            double dx = mouseX - particles[i].x;
                            double dy = mouseY - particles[i].y;
                            double distance = std::sqrt(dx * dx + dy * dy);

                            if (distance < (particles[selectedParticleIndex].radius + particles[i].radius)) {
                                validPosition = false;
                                break;
                            }
                        }
                    }

                    if (mouseX - particles[selectedParticleIndex].radius < 0 ||
                        mouseX + particles[selectedParticleIndex].radius > 1400 ||
                        mouseY - particles[selectedParticleIndex].radius < 50 ||
                        mouseY + particles[selectedParticleIndex].radius > 750) {
                        validPosition = false;
                    }

                    // Jeśli pozycja jest poprawna, ustaw pozycję cząsteczki
                    if (validPosition) {
                        particles[selectedParticleIndex].x = mouseX;
                        particles[selectedParticleIndex].y = mouseY;
                    }
                }
            }
        }

        if (!isPaused) {
            // Aktualizacja pozycji i detekcja kolizji
            for (size_t i = 0; i < particles.size(); i++) {
                if (!gravityEnabled) {
                    particles[i].ay = 0;
                }
                else {
                    particles[i].ay = gravityStrength;  // Użycie zmiennej gravityStrength
                }

                particles[i].update(1400, 750, deltaTime);

                for (size_t j = i + 1; j < particles.size(); j++) {
                    if (particles[i].checkCollision(particles[j])) {
                        particles[i].resolveCollision(particles[j]);
                    }
                }
            }
        }

        // Rysowanie
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Czarny ekran
        SDL_RenderClear(renderer);

        // Rysowanie białego paska tła za przyciskami
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Biały kolor
        SDL_Rect whiteBar = { 0, 0, 1400, 50 }; // Pasek o szerokości całego ekranu i wysokości 50px
        SDL_RenderFillRect(renderer, &whiteBar);

        // Rysowanie przycisków
        SDL_SetRenderDrawColor(renderer, isPaused ? 255 : 200, isPaused ? 0 : 0, 0, 255); // Pause button - czerwona, gdy wstrzymane
        SDL_RenderFillRect(renderer, &pauseButton);
        SDL_SetRenderDrawColor(renderer, gravityEnabled ? 0 : 200, gravityEnabled ? 255 : 0, 0, 255); // Gravity button - zielona, gdy włączona
        SDL_RenderFillRect(renderer, &gravityButton);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &speedSlider);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &sliderHandle);

        // Rysowanie suwaka dla siły grawitacji
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &gravitySlider);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &gravitySliderHandle);

        // Renderowanie tekstu na przyciskach
        SDL_Color textColor = { 0, 0, 0 };  // Kolor tekstu - czarny
        SDL_Surface* surface = nullptr;
        SDL_Texture* texture = nullptr;
        SDL_Rect textRect;

        // Pause Button Text
        surface = TTF_RenderText_Solid(font, "Pause", textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textRect = { pauseButton.x + 10, pauseButton.y + 5, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Gravity Button Text
        surface = TTF_RenderText_Solid(font, gravityEnabled ? "Gravity On" : "Gravity Off", textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textRect = { gravityButton.x + 10, gravityButton.y + 5, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Rysowanie tekstu dla suwaków
        std::stringstream speedText;
        speedText << "Speed: " << timeScale;
        surface = TTF_RenderText_Solid(font, speedText.str().c_str(), textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textRect = { speedSlider.x + 220, speedSlider.y - 5, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Rysowanie tekstu dla grawitacji
        std::stringstream gravityText;
        gravityText << "Gravity: " << gravityStrength;
        surface = TTF_RenderText_Solid(font, gravityText.str().c_str(), textColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textRect = { gravitySlider.x + 220, gravitySlider.y - 5, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Rysowanie cząsteczek
        for (size_t i = 0; i < particles.size(); i++) {
            particles[i].draw(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    // Zwolnienie zasobów
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}