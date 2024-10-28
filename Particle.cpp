#include "Particle.h"

// Konstruktor ustawiaj�cy pozycj�, pr�dko�� i promie� cz�steczki
Particle::Particle(int startX, int startY, int startVx, int startVy, int r)
    : x(startX), y(startY), vx(startVx), vy(startVy), radius(r) {}

// Aktualizacja pozycji i odbicia od �cian
void Particle::update(int screenWidth, int screenHeight) {
    x += vx;
    y += vy;

    // Odbicia od kraw�dzi okna
    if (x - radius <= 0 || x + radius >= screenWidth) {
        vx = -vx; // Odwr�cenie pr�dko�ci w poziomie
    }
    if (y - radius <= 0 || y + radius >= screenHeight) {
        vy = -vy; // Odwr�cenie pr�dko�ci w pionie
    }
}

// Rysowanie cz�steczki jako ma�ego k�ka
void Particle::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Kolor bia�y
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
