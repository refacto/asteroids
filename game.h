#pragma once
#include "objects.h"

struct Game {
	bool isGameOver;
	struct Player player;
	struct Asteroid *asteroids;
};

void game_init(struct Game *game);
void game_destroy(struct Game *game);

void game_update(struct Game *game);
void game_draw(struct Game *game);
