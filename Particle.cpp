#include "Particle.h"
#include <cmath>

Particle::Particle(double x, double y, double vx, double vy, double radius, double mass, Uint8 R, Uint8 G, Uint8 B)
    : x(x), y(y), vx(vx), vy(vy), radius(radius), mass(mass), R(R), G(G), B(B), ax(0), ay(GRAVITY) {}

void Particle::update(int screenWidth, int screenHeight, double deltaTime) {
    // Aktualizacja prêdkoœci na podstawie przyspieszenia
    vx += ax * deltaTime;
    vy += ay * deltaTime;

    // Aktualizacja pozycji na podstawie prêdkoœci i przyspieszenia
    x += vx * deltaTime;
    y += vy * deltaTime + 0.5 * ay * deltaTime * deltaTime;

    // Odbicia od œcian w poziomie
    if (x - radius < 0) {
        vx = -vx * DAMPING; // T³umione odbicie
        x = radius;
    }
    else if (x + radius > screenWidth) {
        vx = -vx * DAMPING; // T³umione odbicie
        x = screenWidth - radius;
    }

    // Odbicia od œcian w pionie
    if (y - radius < 0) {
        vy = -vy * DAMPING; // T³umione odbicie
        y = radius;
    }
    else if (y + radius > screenHeight) {
        vy = -vy * DAMPING; // T³umione odbicie
        y = screenHeight - radius;

        // Jeœli prêdkoœæ jest bardzo ma³a, zatrzymaj cz¹stkê
        if (std::abs(vy) < 0.1) {
            vy = 0;
        }
    }
}

void Particle::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, static_cast<int>(x - radius + w), static_cast<int>(y - radius + h));
            }
        }
    }
}

bool Particle::checkCollision(Particle& other) {
    double dx = other.x - x;
    double dy = other.y - y;
    double distance = std::sqrt(dx * dx + dy * dy);
    return distance < (radius + other.radius);
}

void Particle::resolveCollision(Particle& other) {
    double dx = other.x - x;
    double dy = other.y - y;
    double distance = std::sqrt(dx * dx + dy * dy);

    if (distance == 0.0) return;

    double overlap = (radius + other.radius - distance);
    if (overlap > 0) {
        double correction = 0.5 * overlap / distance;
        x -= correction * dx;
        y -= correction * dy;
        other.x += correction * dx;
        other.y += correction * dy;
    }

    double nx = dx / distance;
    double ny = dy / distance;
    double tx = -ny;
    double ty = nx;

    double v1n = vx * nx + vy * ny;
    double v1t = vx * tx + vy * ty;
    double v2n = other.vx * nx + other.vy * ny;
    double v2t = other.vx * tx + other.vy * ty;

    double v1nFinal = (v1n * (mass - other.mass) + 2 * other.mass * v2n) / (mass + other.mass);
    double v2nFinal = (v2n * (other.mass - mass) + 2 * mass * v1n) / (mass + other.mass);

    vx = v1nFinal * nx + v1t * tx;
    vy = v1nFinal * ny + v1t * ty;
    other.vx = v2nFinal * nx + v2t * tx;
    other.vy = v2nFinal * ny + v2t * ty;
}
