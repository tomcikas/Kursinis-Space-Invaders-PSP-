#include "Alien.h"

Alien::Alien(float startX, float startY, float speed, AlienType type)
    : position({ startX, startY }), speed(speed), active(true), type(type) {
    loadTextureForType();
}

void Alien::loadTextureForType() {
    switch (type) {
    case AlienType::OCTOPUS:
        texture = LoadTexture("textures/octopus.png");
        break;
    case AlienType::CRAB:
        texture = LoadTexture("textures/crab.png");
        break;
    case AlienType::SQUID:
        texture = LoadTexture("textures/squid.png");
        break;
    }

    if (texture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load alien texture for types.");
    }
}

Texture2D Alien::getTexture() const {
    return texture;
}

AlienType Alien::getAlienType() const {
    return type;
}

void Alien::move(int screenWidth) {
    if (!active) return;
    position.x += speed * GetFrameTime();
    if (position.x <= 0 || position.x + texture.width >= screenWidth) {
        speed = -speed;
        position.y += 20;
    }
}

void Alien::moveDown() {
    position.y += 20;
}

void Alien::setSpeed(float newSpeed) {
    speed = newSpeed;
}

void Alien::draw() const {
    if (isActive()) {
        DrawTextureEx(texture, position, 0.0f, 0.56f, WHITE);
    }
}

void Alien::deactivate() {
    active = false;
}


bool Alien::isActive() const {
    return active;
}

Rectangle Alien::getBounds() const {
    return { position.x, position.y, texture.width * 0.56f, texture.height * 0.56f };
}