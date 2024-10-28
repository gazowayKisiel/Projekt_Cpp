#include "Ball.h"

// Konstruktor klasy Ball
Ball::Ball(int startX, int startY, int startVx, int startVy, int r, SDL_Color col)
    : x(startX), y(startY), vx(startVx), vy(startVy), radius(r), color(col) {}

// Funkcja aktualizuj¹ca po³o¿enie kulki i sprawdzaj¹ca odbicia od œcianek
void Ball::update(int screenWidth, int screenHeight) {
    x += vx;
    y += vy;

    // Odbicia od œcianek (lewa, prawa, górna, dolna)
    if (x - radius < 0 || x + radius > screenWidth) {
        vx = -vx;
    }
    if (y - radius < 0 || y + radius > screenHeight) {
        vy = -vy;
    }
}

// Funkcja rysuj¹ca kulkê
void Ball::draw(SDL_Renderer* renderer) {
    // Rysowanie prostego ko³a (kulki) jako wype³nionego okrêgu
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // Odleg³oœæ od œrodka
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_RenderDrawPoint(renderer, x + dx - radius, y + dy - radius);
            }
        }
    }
}
