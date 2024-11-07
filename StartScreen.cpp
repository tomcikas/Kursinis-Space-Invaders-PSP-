#include "StartScreen.h"

StartScreen::StartScreen() {
    titleImage = LoadTexture("textures/startscreen.png");
    if (titleImage.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load end screen texture.");
    }
    else {
        TraceLog(LOG_INFO, "Successfully loaded end screen texture.");
    }
    promptText = "Press SPACEBAR to start";
}

StartScreen::~StartScreen() {
    UnloadTexture(titleImage);
}

void StartScreen::draw() const {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    ClearBackground(BLACK);
    Vector2 titlePosition = {
        static_cast<float>(screenWidth) / 2 - (titleImage.width * 0.500f) / 2,
        static_cast<float>(screenHeight) / 2 - titleImage.height * 0.500f - 50
    };
    DrawTextureEx(titleImage, titlePosition, 0.0f, 0.500f, WHITE);
    const char* promptText = "Press SPACEBAR to start";
    int textY = screenHeight / 2 + 150;
    DrawText(promptText, screenWidth / 2 - MeasureText(promptText, 20) / 2, textY, 20, LIGHTGRAY);
}

bool StartScreen::checkStart() const {
    return IsKeyPressed(KEY_SPACE);
}
