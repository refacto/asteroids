#pragma once
#include "asteroid.h"
#include "fontLoader.h"
#include "healthBar.h"
#include "player.h"
#include "score.h"
#include "scoreboard.h"
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
	struct Score score;
	struct FontLoader *fontLoader;
	struct Scoreboard *scoreboard;
	float noHitTimer;
};

void game_init(struct Game *game, struct SoundFx *sfx,
			   struct FontLoader *fontLoader, struct Scoreboard *scoreboard);
void game_destroy(struct Game *game);
void game_reset(struct Game *game);

void game_update(struct Game *game, struct ScreenController *ctrl);
void game_draw(struct Game *game);

void game_screen_update(struct ScreenController *ctrl, void *data);
void game_screen_draw(void *data);
