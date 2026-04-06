#pragma once
#include "player.h"
#include "objects.h"
#include "screenController.h"
#include <raylib.h>

struct Game {
	bool isGameOver;
	struct Player player;
	struct Asteroid *asteroids;
	Vector2 screen_dimensions;
};

void game_init(struct Game *game, int screenWidth, int screenHeight);
void game_destroy(struct Game *game);

void game_update(struct Game *game);
void game_draw(struct Game *game);

void game_screen_update(struct ScreenController *ctrl, void *data);
void game_screen_draw(struct ScreenController *ctrl, void *data);
