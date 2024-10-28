#pragma once
#ifndef BALL_H
#define BALL_H
#include <SDL.h>

// Klasa reprezentuj¹ca kulkê
class Ball {
public:
    int x, y;  // Pozycja kulki
    int vx, vy; // Prêdkoœæ kulki
    int radius; // Promieñ kulki
    SDL_Color color; // Kolor kulki

    Ball(int startX, int startY, int startVx, int startVy, int r, SDL_Color col);

    // Funkcja aktualizuj¹ca po³o¿enie kulki i sprawdzaj¹ca odbicia od œcianek
    void update(int screenWidth, int screenHeight);

    // Funkcja rysuj¹ca kulkê
    void draw(SDL_Renderer* renderer);
};

#endif // BALL_H
