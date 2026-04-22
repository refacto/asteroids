#pragma once
#include "asteroid.h"
#include "player.h"
#include "screenController.h"
#include "shot.h"
#include <raylib.h>

constexpr int MAX_NUM_SHOTS = 5;

struct Game {
	bool isGameOver;
	struct Player player;
	struct Asteroid *asteroids;
	struct Shot shots[MAX_NUM_SHOTS];
	Vector2 screen_dimensions;
};

void game_init(struct Game *game, int screenWidth, int screenHeight);
void game_destroy(struct Game *game);

void game_update(struct Game *game);
void game_draw(struct Game *game);

void game_screen_update(struct ScreenController *ctrl, void *data);
void game_screen_draw(void *data);
