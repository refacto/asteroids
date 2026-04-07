#pragma once
#include "objects.h"
#include "shot.h"
#include <raylib.h>

constexpr int MAX_NUM_SHOTS = 5;

struct Player {
	struct Object object;
	struct Shot shots[MAX_NUM_SHOTS];
	int lives;
	Sound shoot_sound;
};

void player_init(struct Player *player, Vector2 screen_dimensions);
void player_destroy(struct Player *player);
void player_update(struct Player *player);
void player_draw(struct Player *player);
void player_move(struct Player *player, Vector2 screen_dimensions);
