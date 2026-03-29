#pragma once
#include "player.h"
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
