#include <iostream>
#include <vector>
#include "raylib.h"
#include "Player.h"
#include "Wave.h"
#include "Bullet.h"
#include "Shield.h"
#include "StartScreen.h"
#include "EndScreen.h"
#include "Score.h"


int playerLives = 3;
bool playerIsDead = false;
const float shieldYPosition = 600.0f;

void resetGame(int& playerLives, bool& playerIsDead) {
    playerLives = 3;
    playerIsDead = false;
}

Score score;

void addPointsBasedOnAlienType(const AlienType& alienType) {
    int points = 0;
    if (alienType == AlienType::OCTOPUS) {
        points = 10;
    }
    else if (alienType == AlienType::CRAB) {
        points = 20;
    }
    else if (alienType == AlienType::SQUID) {
        points = 30;
    }
    score.addPoints(points);
}

enum GameState {
    START,
    PLAYING,
    GAMEOVER,
    WIN
};

int main() {
    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Space Invaders v.1.0.0");
    Texture2D background = LoadTexture("backgrounds/background5.png");
    Player player(screenWidth / 2 - 20, screenHeight - 50, 300.0f, 3);
    AlienWave wave(screenWidth, screenHeight);
    StartScreen startScreen;
	EndScreen endScreen;
    std::vector<Bullet> playerBullets(10);
    std::vector<Bullet> alienBullets(10);
    GameState gameState = START;
    bool gameOver = false;

    Shield shield1(240, 600, 3);
    Shield shield2(480, 600, 3);
    Shield shield3(720, 600, 3);
    Shield shield4(960, 600, 3);

    float backgroundScaleX = static_cast<float>(GetScreenWidth()) / background.width;
    float backgroundScaleY = static_cast<float>(GetScreenHeight()) / background.height;

    SetTargetFPS(144);

    while (!WindowShouldClose()) {
        bool allAliensDead = true;
        switch (gameState) {
        case START:
            startScreen.draw();
            if (startScreen.checkStart()) {
                gameState = PLAYING;
                player.reset();
                wave.reset();
                score.reset();
				shield1.reset();
				shield2.reset();
				shield3.reset();
				shield4.reset();
            }
            break;

        case PLAYING:
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTextureEx(background, { 0, 0 }, 0.0f, fmax(backgroundScaleX, backgroundScaleY), WHITE);

            for (const auto& row : wave.aliens) {
                for (const auto& alien : row) {
                    if (alien.isActive()) {
                        allAliensDead = false;
                        break;
                    }
                }
                if (!allAliensDead) break;
            }

            if (allAliensDead) {
                player.reset();
                wave.reset();
                shield1.reset();
                shield2.reset();
                shield3.reset();
                shield4.reset();
            }

            for (const auto& row : wave.aliens) {
                for (const auto& alien : row) {
                    if (alien.isActive() && alien.getBounds().y + alien.getBounds().height >= shieldYPosition) {
                        gameOver = true;
                        break;
                    }
                }
                if (gameOver) break;
            }

            if (gameOver) {
                gameState = GAMEOVER;
                break;
            }

            player.move(screenWidth);
            wave.draw();
            wave.update();
            player.draw();
            player.shoot(playerBullets);

            shield1.draw();
            shield2.draw();
            shield3.draw();
            shield4.draw();

            for (auto& bullet : playerBullets) {
                bullet.move();

            }

            for (const auto& bullet : playerBullets) {
                bullet.draw();
            }

            for (const auto& alienBullet : alienBullets) {
                alienBullet.draw();
            }

            // Collision check between player's bullets and aliens
            for (auto& bullet : playerBullets) {
                if (bullet.isActive()) {
                    Alien* hitAlien = nullptr;
                    for (auto& row : wave.aliens) {
                        for (auto& alien : row) {
                            if (alien.isActive() && CheckCollisionRecs(alien.getBounds(), bullet.getBounds())) {
                                addPointsBasedOnAlienType(alien.getAlienType());
                                hitAlien = &alien;
                                bullet.active = false;
                                break;
                            }
                        }
                        if (hitAlien) break;
                    }
                    if (hitAlien) {
                        hitAlien->deactivate();
                    }
                }
            }

            // Collision check between alien bullets and player
            for (auto& alienBullet : wave.alienBullets) {
                if (alienBullet.isActive()) {
                    if (alienBullet.checkCollision(player.getBounds())) {
                        player.reduceLife();
                        alienBullet.active = false;
                    }
                }
            }

            // Collision check for player bullets with shields
            for (auto& bullet : playerBullets) {
                if (bullet.isActive()) {

                    Vector2 bulletPos = bullet.getPosition();
                    const std::vector<std::vector<int>>& shield1Grid = shield1.getGrid();
                    Vector2 shield1Position = shield1.getPosition();
                    float shield1BlockSize = shield1.getBlockSize();

                    for (int row = 0; row < shield1Grid.size(); row++) {
                        for (int col = 0; col < shield1Grid[row].size(); col++) {
                            if (shield1Grid[row][col] == 1) {
                                Rectangle shieldBlock = {
                                    shield1Position.x + col * shield1BlockSize,
                                    shield1Position.y + row * shield1BlockSize,
                                    shield1BlockSize,
                                    shield1BlockSize
                                };
                                if (bullet.checkCollision(shieldBlock)) {
                                    bullet.active = false;
                                    break;
                                }
                            }
                        }
                        if (!bullet.isActive()) break;
                    }

                    const std::vector<std::vector<int>>& shield2Grid = shield2.getGrid();
                    Vector2 shield2Position = shield2.getPosition();
                    float shield2BlockSize = shield2.getBlockSize();

                    for (int row = 0; row < shield2Grid.size(); row++) {
                        for (int col = 0; col < shield2Grid[row].size(); col++) {
                            if (shield2Grid[row][col] == 1) {
                                Rectangle shieldBlock = {
                                    shield2Position.x + col * shield2BlockSize,
                                    shield2Position.y + row * shield2BlockSize,
                                    shield2BlockSize,
                                    shield2BlockSize
                                };
                                if (bullet.checkCollision(shieldBlock)) {
                                    bullet.active = false;
                                    break;
                                }
                            }
                        }
                        if (!bullet.isActive()) break;
                    }

                    const std::vector<std::vector<int>>& shield3Grid = shield3.getGrid();
                    Vector2 shield3Position = shield3.getPosition();
                    float shield3BlockSize = shield3.getBlockSize();

                    for (int row = 0; row < shield3Grid.size(); row++) {
                        for (int col = 0; col < shield3Grid[row].size(); col++) {
                            if (shield3Grid[row][col] == 1) {
                                Rectangle shieldBlock = {
                                    shield3Position.x + col * shield3BlockSize,
                                    shield3Position.y + row * shield3BlockSize,
                                    shield3BlockSize,
                                    shield3BlockSize
                                };
                                if (bullet.checkCollision(shieldBlock)) {
                                    bullet.active = false;
                                    break;
                                }
                            }
                        }
                        if (!bullet.isActive()) break;
                    }

                    const std::vector<std::vector<int>>& shield4Grid = shield4.getGrid();
                    Vector2 shield4Position = shield4.getPosition();
                    float shield4BlockSize = shield4.getBlockSize();

                    for (int row = 0; row < shield4Grid.size(); row++) {
                        for (int col = 0; col < shield4Grid[row].size(); col++) {
                            if (shield4Grid[row][col] == 1) {
                                Rectangle shieldBlock = {
                                    shield4Position.x + col * shield4BlockSize,
                                    shield4Position.y + row * shield4BlockSize,
                                    shield4BlockSize,
                                    shield4BlockSize
                                };

                                if (bullet.checkCollision(shieldBlock)) {
                                    bullet.active = false;
                                    break;
                                }
                            }
                        }
                        if (!bullet.isActive()) break;
                    }
                }
            }

			// Collision check between alien bullets and shields
            for (auto& alienBullet : wave.alienBullets) {
                if (alienBullet.isActive()) {

                    Vector2 alienBulletPos = alienBullet.getPosition();
                    const std::vector<std::vector<int>>& shield1Grid = shield1.getGrid();
                    Vector2 shield1Position = shield1.getPosition();
                    float shield1BlockSize = shield1.getBlockSize();

                    for (int row = 0; row < shield1Grid.size(); row++) {
                        for (int col = 0; col < shield1Grid[row].size(); col++) {
                            if (shield1Grid[row][col] == 1) {
                                Rectangle shieldBlock = {
                                    shield1Position.x + col * shield1BlockSize,
                                    shield1Position.y + row * shield1BlockSize,
                                    shield1BlockSize,
                                    shield1BlockSize
                                };
                                if (alienBullet.checkCollision(shieldBlock)) {
                                    shield1.takeDamage(static_cast<int>(alienBulletPos.x), static_cast<int>(alienBulletPos.y), 5);
                                    alienBullet.active = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            for (auto& alienBullet : wave.alienBullets) {
                if (alienBullet.isActive()) {

                    Vector2 alienBulletPos = alienBullet.getPosition();
                    const std::vector<std::vector<int>>& shield2Grid = shield2.getGrid();
                    Vector2 shield2Position = shield2.getPosition();
                    float shield2BlockSize = shield2.getBlockSize();

                    for (int row = 0; row < shield2Grid.size(); row++) {
                        for (int col = 0; col < shield2Grid[row].size(); col++) {
                            if (shield2Grid[row][col] == 1) {
                                Rectangle shieldBlock = {
                                    shield2Position.x + col * shield2BlockSize,
                                    shield2Position.y + row * shield2BlockSize,
                                    shield2BlockSize,
                                    shield2BlockSize
                                };
                                if (alienBullet.checkCollision(shieldBlock)) {
                                    shield2.takeDamage(static_cast<int>(alienBulletPos.x), static_cast<int>(alienBulletPos.y), 5);
                                    alienBullet.active = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            for (auto& alienBullet : wave.alienBullets) {
                if (alienBullet.isActive()) {

                    Vector2 alienBulletPos = alienBullet.getPosition();
                    const std::vector<std::vector<int>>& shield3Grid = shield3.getGrid();
                    Vector2 shield3Position = shield3.getPosition();
                    float shield3BlockSize = shield3.getBlockSize();

                    for (int row = 0; row < shield3Grid.size(); row++) {
                        for (int col = 0; col < shield3Grid[row].size(); col++) {
                            if (shield3Grid[row][col] == 1) {
                                Rectangle shieldBlock = {
                                    shield3Position.x + col * shield3BlockSize,
                                    shield3Position.y + row * shield3BlockSize,
                                    shield3BlockSize,
                                    shield3BlockSize
                                };
                                if (alienBullet.checkCollision(shieldBlock)) {
                                    shield3.takeDamage(static_cast<int>(alienBulletPos.x), static_cast<int>(alienBulletPos.y), 5);
                                    alienBullet.active = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            for (auto& alienBullet : wave.alienBullets) {
                if (alienBullet.isActive()) {

                    Vector2 alienBulletPos = alienBullet.getPosition();
                    const std::vector<std::vector<int>>& shield4Grid = shield4.getGrid();
                    Vector2 shield4Position = shield4.getPosition();
                    float shield4BlockSize = shield4.getBlockSize();

                    for (int row = 0; row < shield4Grid.size(); row++) {
                        for (int col = 0; col < shield4Grid[row].size(); col++) {
                            if (shield4Grid[row][col] == 1) {
                                Rectangle shieldBlock = {
                                    shield4Position.x + col * shield4BlockSize,
                                    shield4Position.y + row * shield4BlockSize,
                                    shield4BlockSize,
                                    shield4BlockSize
                                };
                                if (alienBullet.checkCollision(shieldBlock)) {
                                    shield4.takeDamage(static_cast<int>(alienBulletPos.x), static_cast<int>(alienBulletPos.y), 5);
                                    alienBullet.active = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            DrawText(TextFormat("Highscore: %d", score.getScore()), 10, 40, 20, WHITE);
            DrawText(TextFormat("Lives: %d", player.getLives()), 10, 10, 20, WHITE);
            
            if (player.getLives() <= 0) {
                gameState = GAMEOVER;
            }
            break;

        case GAMEOVER:
            endScreen.draw();
            if (endScreen.checkRestart()) {
                resetGame(playerLives, playerIsDead);
                gameState = START;
            }

            const char* scoreText = TextFormat("Your score: %d", score.getScore());
            int fontSize = 35;

            int textWidth = MeasureText(scoreText, fontSize);
            int positionX = (GetScreenWidth() - textWidth) / 2;
            int positionY = GetScreenHeight() / 2 + 95;

            DrawText(scoreText, positionX, positionY, fontSize, RED);
            break;
        }
        ClearBackground(BLACK);
        EndDrawing();
    }
    UnloadTexture(background);
    CloseWindow();
    return 0;
}
