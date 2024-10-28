#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL.h>

class Particle {
public:
    Particle(int startX, int startY, int startVx, int startVy, int r);

    void update(int screenWidth, int screenHeight);  // Aktualizuje pozycjê i odbicia od œcian
    void draw(SDL_Renderer* renderer);               // Rysuje cz¹steczkê

private:
    int x, y;       // Pozycja cz¹steczki
    int vx, vy;     // Prêdkoœæ cz¹steczki
    int radius;     // Promieñ cz¹steczki
};

#endif
