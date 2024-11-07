#pragma once

#include <vector>
#include "Alien.h"
#include "Bullet.h"

class AlienWave {
public:
    AlienWave(int screenWidth, int screenHeight);
    void update();
    void draw() const;
    void reset();
    std::vector<Bullet> alienBullets;
    std::vector<std::vector<Alien>> aliens;

private:
    int screenWidth;
    int screenHeight;
    float alienSpeed;
    bool directionChanged;
    Vector2 baseOffset;
    void initializeAliens();
    bool checkWallCollision() const;
    void alienShoot();
};
