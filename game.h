#pragma once
#include "asteroid.h"
#include "healthBar.h"
#include "player.h"
#include "screenController.h"
#include "shot.h"
#include "soundFx.h"
#include <raylib.h>

constexpr int MAX_NUM_SHOTS = 5;

struct Game {
	bool isGameOver;
	struct Player player;
	struct Asteroid *asteroids;
	struct Shot shots[MAX_NUM_SHOTS];
	struct HealthBar healthBar;
	struct SoundFx *sfx;
};

void game_init(struct Game *game, struct SoundFx *sfx);
void game_destroy(struct Game *game);

void game_update(struct Game *game);
void game_draw(struct Game *game);

void game_screen_update(struct ScreenController *ctrl, void *data);
void game_screen_draw(void *data);
