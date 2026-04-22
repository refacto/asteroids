#pragma once

#include "asteroidShower.h"
#include "fontLoader.h"
#include "screenController.h"
#include <raylib.h>

struct Title {
	bool isGameOver;
	Vector2 screenDimensions;
	struct FontLoader *fontLoader;
	struct AsteroidShower asteroidShower;
};

void title_init(struct Title *title, struct FontLoader *fontLoader,
				int screenWidth, int screenHeight);
void title_destroy(struct Title *title);

void title_update(struct Title *title, struct ScreenController *ctrl);
void title_draw(struct Title *title);

void title_screen_update(struct ScreenController *ctrl, void *data);
void title_screen_draw(void *data);
