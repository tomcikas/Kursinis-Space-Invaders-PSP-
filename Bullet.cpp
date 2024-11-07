#include "Bullet.h"

Bullet::Bullet() {
    position = { 0.0f, 0.0f };
    speed = 500.0f;
    active = false;
    width = 5.0f;
    height = 10.0f;
}

float Bullet::getWidth() const {
    return width;
}

float Bullet::getHeight() const {
    return height;
}

bool Bullet::isActive() const {
    return active;
}

Rectangle Bullet::getBounds() const {
	return { position.x, position.y, width, height };
}

Vector2 Bullet::getPosition() const {
    return position;
}

void Bullet::move() {
    if (active) {
        position.y -= speed * GetFrameTime();
        if (position.y < 0) {
            active = false;
        }
    }
}

void Bullet::draw() const {
    if (active) {
        DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(width), static_cast<int>(height), BLUE);
    }
}

bool Bullet::checkCollision(Rectangle target) const {
    if (active) {
        Rectangle bulletRectangle = { position.x, position.y, width, height };
        return CheckCollisionRecs(bulletRectangle, target);
    }
    return false;
}
