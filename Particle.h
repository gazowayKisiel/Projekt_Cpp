#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL.h>

class Particle {
public:
    Particle(int startX, int startY, int startVx, int startVy, int r);

    void update(int screenWidth, int screenHeight);  // Aktualizuje pozycj� i odbicia od �cian
    void draw(SDL_Renderer* renderer);               // Rysuje cz�steczk�

private:
    int x, y;       // Pozycja cz�steczki
    int vx, vy;     // Pr�dko�� cz�steczki
    int radius;     // Promie� cz�steczki
};

#endif
