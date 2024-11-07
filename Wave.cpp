#include "Wave.h"
#include "raylib.h"
#include <cstdlib>
#include <ctime>


AlienWave::AlienWave(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight), alienSpeed(100.0f), directionChanged(false) {
    initializeAliens();
    srand(static_cast<unsigned>(time(0)));
    alienBullets.resize(20);
}

void AlienWave::reset() {
    aliens.clear();
    alienBullets.clear();
    baseOffset = { 0, 0 };
    directionChanged = false;
    alienSpeed = 100.0f;
    initializeAliens();
    alienBullets.resize(20);

    for (auto& bullet : alienBullets) {
        bullet.active = false;
    }
}

void AlienWave::initializeAliens() {
    const int alienRows = 5;
    const int alienCols = 11;
    const float alienSpacingX = 40.0f;
    const float alienSpacingY = 40.0f;

    for (int row = 0; row < alienRows; row++) {
        std::vector<Alien> alienRow;
        AlienType type;
        if (row == 0) {
            type = AlienType::SQUID;
        }
        else if (row <= 2) {
            type = AlienType::CRAB;
        }
        else {
            type = AlienType::OCTOPUS;
        }

        for (int col = 0; col < alienCols; col++) {
            float startX = 100.0f + col * alienSpacingX;
            float startY = 50.0f + row * alienSpacingY;
            alienRow.emplace_back(startX, startY, alienSpeed, type);
        }
        aliens.push_back(alienRow);
    }
}

void AlienWave::alienShoot() {
    std::vector<Alien*> eligibleShooters;

    const int alienCols = 11;
    const int alienRows = aliens.size();

    for (int col = 0; col < alienCols; col++) {
        Alien* shooter = nullptr;

        // Start from the bottom row and move up to find the lowest active alien in this column
        for (int row = alienRows - 1; row >= 0; row--) {
            if (aliens[row][col].isActive()) {
                shooter = &aliens[row][col];
                break;
            }
        }
        if (shooter) {
            eligibleShooters.push_back(shooter);
        }
    }

    // If there are eligible shooters, pick a random one to shoot
    if (!eligibleShooters.empty()) {
        int shooterIndex = rand() % eligibleShooters.size();
        Alien* shooter = eligibleShooters[shooterIndex];
        for (auto& bullet : alienBullets) {
            if (!bullet.active) {
                bullet.position = {
                    shooter->getBounds().x + shooter->getBounds().width / 2 + baseOffset.x,
                    shooter->getBounds().y + shooter->getBounds().height + baseOffset.y
                };
                bullet.active = true;
                bullet.speed = 200.0f;
                break;
            }
        }
    }
}

bool AlienWave::checkWallCollision() const {
    for (const auto& row : aliens) {
        for (const auto& alien : row) {
            if (alien.isActive()) {
                float alienX = alien.getBounds().x + baseOffset.x;
                if (alienX <= 0 || alienX + alien.getBounds().width >= screenWidth) {
                    return true;
                }
            }
        }
    }
    return false;
}

void AlienWave::update() {
    bool atEdge = checkWallCollision();
    if (atEdge && !directionChanged) {
        alienSpeed = -alienSpeed;
        for (auto& row : aliens) {
            for (auto& alien : row) {
                alien.position.y += 20;
            }
        }
        directionChanged = true;
    }
    else if (!atEdge) {
        directionChanged = false;
    }

    for (auto& row : aliens) {
        for (auto& alien : row) {
            alien.position.x += alienSpeed * GetFrameTime();
        }
    }

    if (rand() % 100 < 2) {
        alienShoot();
    }

    for (auto& bullet : alienBullets) {
        if (bullet.active) {
            bullet.position.y += bullet.speed * GetFrameTime();
            if (bullet.position.y > screenHeight) {
                bullet.active = false;
            }
        }
    }
}

void AlienWave::draw() const {
    for (const auto& row : aliens) {
        for (const auto& alien : row) {
            if (alien.isActive()) {
                DrawTextureEx(alien.getTexture(), alien.position, 0.0f, 0.56f, WHITE);
            }
        }
    }
    for (const auto& bullet : alienBullets) {
        if (bullet.active) {
            DrawRectangle(static_cast<int>(bullet.position.x), static_cast<int>(bullet.position.y), 5, 10, RED);
        }
    }
}

