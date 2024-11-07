#pragma once
#include "raylib.h"

class Bullet {
public:
    Bullet();
    void move();
    void draw() const;
    bool checkCollision(Rectangle target) const;
    Rectangle getBounds() const;
    bool isActive() const;
    Vector2 getPosition() const;
    Vector2 position;
    float speed;
    bool active;
    float getWidth() const;
    float getHeight() const;

private:
    float width;
    float height;
};
