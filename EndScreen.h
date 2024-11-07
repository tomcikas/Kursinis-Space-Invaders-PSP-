#pragma once
#include "raylib.h"

class EndScreen {
public:
    EndScreen();
    ~EndScreen();
    void draw() const;
    bool checkRestart() const;

private:
    Texture2D endImage;
    const char* endText;
};
