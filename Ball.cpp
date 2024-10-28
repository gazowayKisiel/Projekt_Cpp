#include "Ball.h"

// Konstruktor klasy Ball
Ball::Ball(int startX, int startY, int startVx, int startVy, int r, SDL_Color col)
    : x(startX), y(startY), vx(startVx), vy(startVy), radius(r), color(col) {}

// Funkcja aktualizuj�ca po�o�enie kulki i sprawdzaj�ca odbicia od �cianek
void Ball::update(int screenWidth, int screenHeight) {
    x += vx;
    y += vy;

    // Odbicia od �cianek (lewa, prawa, g�rna, dolna)
    if (x - radius < 0 || x + radius > screenWidth) {
        vx = -vx;
    }
    if (y - radius < 0 || y + radius > screenHeight) {
        vy = -vy;
    }
}

// Funkcja rysuj�ca kulk�
void Ball::draw(SDL_Renderer* renderer) {
    // Rysowanie prostego ko�a (kulki) jako wype�nionego okr�gu
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // Odleg�o�� od �rodka
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_RenderDrawPoint(renderer, x + dx - radius, y + dy - radius);
            }
        }
    }
}
