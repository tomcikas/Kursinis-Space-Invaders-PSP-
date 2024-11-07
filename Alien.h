#pragma once
#include "raylib.h"

enum class AlienType {
    OCTOPUS,
    CRAB,
    SQUID
};

class Alien {
public:
    Alien(float startX, float startY, float speed, AlienType type);
    void move(int screenWidth);
    void draw() const;
    void moveDown();
    void setSpeed(float newSpeed);
    void deactivate();
    bool isActive() const;
    AlienType getAlienType() const;
    Rectangle getBounds() const;
    Texture2D getTexture() const;
    Vector2 position;

private:
    float speed;
    bool active;
    Texture2D texture;
    AlienType type;
    void loadTextureForType();
};