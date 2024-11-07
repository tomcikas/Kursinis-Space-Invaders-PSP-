#pragma once
#include "raylib.h"
#include <vector>

class Shield {
public:
    Shield(float x, float y, float blockSize);
    void draw() const;
    void takeDamage(int hitX, int hitY, int damageRadius);
    float getWidth() const;
    float getHeight() const;
    void reset();
    const std::vector<std::vector<int>>& getGrid() const;
    Vector2 getPosition() const;
    float getBlockSize() const;

private:
    Vector2 position;
    float blockSize;
    std::vector<std::vector<int>> grid;
};