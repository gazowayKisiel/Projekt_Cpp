#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Particle.h"

int main(int argc, char* argv[]) {
    // Inicjalizacja SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Particle Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
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

    std::srand(std::time(nullptr));
    std::vector<Particle> particles;

    // Tworzenie cząsteczek
    for (int i = 0; i < 8; i++) {
        int radius = 10;
        int x = std::rand() % (800 - 2 * radius) + radius;
        int y = std::rand() % (600 - 2 * radius) + radius;
        int vx = (std::rand() % 5 + 1) * (std::rand() % 2 == 0 ? 1 : -1);
        int vy = (std::rand() % 5 + 1) * (std::rand() % 2 == 0 ? 1 : -1);

        particles.push_back(Particle(x, y, vx, vy, radius));
    }

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Aktualizacja pozycji cząsteczek
        for (auto& particle : particles) {
            particle.update(800, 600);
        }

        // Rysowanie cząsteczek
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Czarny ekran
        SDL_RenderClear(renderer);

        for (auto& particle : particles) {
            particle.draw(renderer);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Krótkie opóźnienie dla płynności
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
