#include "Particle.h"

// Konstruktor ustawiaj¹cy pozycjê, prêdkoœæ i promieñ cz¹steczki
Particle::Particle(int startX, int startY, int startVx, int startVy, int r)
    : x(startX), y(startY), vx(startVx), vy(startVy), radius(r) {}

// Aktualizacja pozycji i odbicia od œcian
void Particle::update(int screenWidth, int screenHeight) {
    x += vx;
    y += vy;

    // Odbicia od krawêdzi okna
    if (x - radius <= 0 || x + radius >= screenWidth) {
        vx = -vx; // Odwrócenie prêdkoœci w poziomie
    }
    if (y - radius <= 0 || y + radius >= screenHeight) {
        vy = -vy; // Odwrócenie prêdkoœci w pionie
    }
}

// Rysowanie cz¹steczki jako ma³ego kó³ka
void Particle::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Kolor bia³y
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx - radius, y + dy - radius);
            }
        }
    }
}
