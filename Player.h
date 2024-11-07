#pragma once
#include "raylib.h"
#include <vector>
#include "Bullet.h"



class Player {
public:
	Player(float startX, float startY, float speed, int lives);
	~Player();
	void move(int screenWidth);
	void shoot(std::vector<Bullet>& bullets);
	void draw() const;
	void reset();
	Vector2 getPosition() const;
	int getLives() const;
	void reduceLife();
	Rectangle getBounds() const;

private:
	Vector2 position;
	float speed;
	int lives;
	Texture2D texture;
	
};
