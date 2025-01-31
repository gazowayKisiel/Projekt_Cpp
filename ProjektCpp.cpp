#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "Particle.h"
#include "Interface.h"


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
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

    Interface interface(renderer, 1400, 750);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<Particle> particles;

    // Tworzenie cząsteczek
    for (int i = 0; i < 19; i++) {
        double mass = 2 * (10.0 + static_cast<double>(std::rand() % 10));
        double radius = mass / 2;
        double x = std::rand() % (1400 - static_cast<int>(2 * radius)) + radius;
        double y = std::rand() % (750 - static_cast<int>(2 * radius)) + radius;
        double vx = (std::rand() % 50 + 1) * (std::rand() % 2 == 0 ? 1 : -1);
        double vy = (std::rand() % 50 + 1) * (std::rand() % 2 == 0 ? 1 : -1);
        Uint8 r = std::rand() % 256;
        Uint8 g = std::rand() % 256;
        Uint8 b = std::rand() % 256;

        particles.emplace_back(x, y, vx, vy, radius, mass, r, g, b);
    }

    particles.emplace_back(100, 100, 0, 0, 40, 100000000000, 254, 254, 254);

    bool quit = false;
    bool isPaused = false; // Flaga pauzy
    bool isDragging = false; // Flaga przeciągania
    int selectedParticleIndex = -1; // Indeks zaznaczonej cząsteczki
    SDL_Event e;

    Uint32 lastTime = SDL_GetTicks();
    double timeScale = 2.0; // Współczynnik przyspieszenia symulacji

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - lastTime) / 1000.0 * timeScale; // Obliczenie czasu z uwzględnieniem przyspieszenia
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



            else if (e.type == SDL_MOUSEBUTTONDOWN && isPaused) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Sprawdzanie, czy kliknięto na cząsteczkę
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
            else if (e.type == SDL_MOUSEBUTTONUP && isPaused) {
                isDragging = false; // Zatrzymanie przeciągania
            }
            else if (e.type == SDL_MOUSEMOTION && isDragging && isPaused && selectedParticleIndex != -1) {
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
                    mouseY - particles[selectedParticleIndex].radius < 0 ||
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

        if (!isPaused) {
            // Aktualizacja pozycji i detekcja kolizji
            for (size_t i = 0; i < particles.size(); i++) {
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

        for (size_t i = 0; i < particles.size(); i++) {
            particles[i].draw(renderer);

            // Rysowanie obramowania dla zaznaczonej cząsteczki
            if (static_cast<int>(i) == selectedParticleIndex) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Biały kolor
                for (int w = -2; w <= 2; ++w) { // Grubość obramowania
                    for (double angle = 0; angle <= 360; angle += 1) {
                        double rad = angle * M_PI / 180;
                        int borderX = static_cast<int>(particles[i].x + (particles[i].radius + w) * std::cos(rad));
                        int borderY = static_cast<int>(particles[i].y + (particles[i].radius + w) * std::sin(rad));
                        SDL_RenderDrawPoint(renderer, borderX, borderY);
                    }
                }
            }
        }

        // Wyświetlenie komunikatu o pauzie
        if (isPaused) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Ustaw kolor na biały
            SDL_Rect pauseRect1 = { 350, 280, 20, 40 };  // Pierwszy pionowy prostokąt
            SDL_Rect pauseRect2 = { 375, 280, 20, 40 };  // Drugi pionowy prostokąt

            SDL_RenderFillRect(renderer, &pauseRect1);
            SDL_RenderFillRect(renderer, &pauseRect2);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(8); // Krótsze opóźnienie dla płynniejszego działania
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
