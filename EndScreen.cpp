#include "EndScreen.h"

EndScreen::EndScreen() {
    endImage = LoadTexture("textures/gameoverscreen.png");
    if (endImage.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load end screen texture.");
    }
    else {
        TraceLog(LOG_INFO, "Successfully loaded end screen texture.");
    }
    endText = "Press ENTER to restart";
}

EndScreen::~EndScreen() {
    if (endImage.id != 0) {
        UnloadTexture(endImage);
    }
}

void EndScreen::draw() const {
    DrawTexture(endImage, GetScreenWidth() / 2 - endImage.width / 2, GetScreenHeight() / 2 - endImage.height / 2 - 65, WHITE);
    DrawText(endText, GetScreenWidth() / 2 - MeasureText(endText, 20) / 2, GetScreenHeight() / 2 + endImage.height / 2 + 20, 20, WHITE);
}

bool EndScreen::checkRestart() const {
    return IsKeyPressed(KEY_ENTER);
}