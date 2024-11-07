#pragma once
#include "raylib.h"


class StartScreen {
public:
	StartScreen();
	~StartScreen();
	void draw() const;
	bool checkStart() const;

private:
	Texture2D titleImage;
	const char* promptText;
};

