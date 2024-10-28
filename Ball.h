#pragma once
#ifndef BALL_H
#define BALL_H
#include <SDL.h>

// Klasa reprezentuj�ca kulk�
class Ball {
public:
    int x, y;  // Pozycja kulki
    int vx, vy; // Pr�dko�� kulki
    int radius; // Promie� kulki
    SDL_Color color; // Kolor kulki

    Ball(int startX, int startY, int startVx, int startVy, int r, SDL_Color col);

    // Funkcja aktualizuj�ca po�o�enie kulki i sprawdzaj�ca odbicia od �cianek
    void update(int screenWidth, int screenHeight);

    // Funkcja rysuj�ca kulk�
    void draw(SDL_Renderer* renderer);
};

#endif // BALL_H
