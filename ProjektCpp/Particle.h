#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL.h>
#include <cmath>

class Particle {
public:
    Particle(double startX, double startY, double startVx, double startVy, double r, double m, Uint8 R, Uint8 G, Uint8 B);
    void update(int screenWidth, int screenHeight, double deltaTime);
    void draw(SDL_Renderer* renderer);
    bool checkCollision(Particle& other);
    void resolveCollision(Particle& other);

    double x, y;       // Pozycje
    double vx, vy;     // Pr�dko�ci
    double ax, ay;     // Przyspieszenia
    double radius;     // Promie�
    double mass;       // Masa
    Uint8 R, G, B;     // Kolor
    static constexpr double GRAVITY = 9.81; // Przyspieszenie grawitacyjne
    static constexpr double DAMPING = 1.01; // Wsp�czynnik t�umienia odbicia
};

#endif