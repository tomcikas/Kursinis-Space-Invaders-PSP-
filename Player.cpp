#include "Player.h"
#include "raylib.h"
#include "Bullet.h"

Player::Player(float startX, float startY, float speed, int lives)
	: position({ startX, startY }), speed(speed), lives(lives) {
	texture = LoadTexture("textures/spaceship.png");
	if (texture.id == 0) {
		TraceLog(LOG_ERROR, "Failed to load player texture.");
	}
	else {
		TraceLog(LOG_INFO, "Successfully loaded player texture.");
	}
}

Player::~Player() {
	if (texture.id != 0) UnloadTexture(texture);
}

void Player::reset() {
	lives = 3;
	position = { static_cast<float>(GetScreenWidth()) / 2, static_cast<float>(GetScreenHeight()) - 50.0f };
	std::vector<Bullet> bullets(10);
	for (auto& bullet : bullets) {
		bullet.active = false;
	}
}

void Player::move(int screenWidth) {
	if (IsKeyDown(KEY_RIGHT) && position.x < screenWidth - 40) {
		position.x += speed * GetFrameTime();
	}
	if (IsKeyDown(KEY_LEFT) && position.x > 0) {
		position.x -= speed * GetFrameTime();
	}
}

void Player::shoot(std::vector<Bullet>& bullets) {
	if (IsKeyPressed(KEY_SPACE)) {
		for (auto& bullet : bullets) {
			if (!bullet.active) {
				bullet.position = { position.x + (texture.width * 0.054f) / 2 - bullet.getWidth() / 2, position.y};
				bullet.active = true;
				break;
			}
		}
	}
}

Rectangle Player::getBounds() const {
	return { position.x, position.y, texture.width * 0.054f, texture.height * 0.054f };
}

void Player::draw() const {
	DrawTextureEx(texture, position, 0.0f, 0.054f, WHITE);
}

Vector2 Player::getPosition() const {
	return position;
}

int Player::getLives() const {
	return lives;
}

void Player::reduceLife() {
	lives--;
}