#pragma once
#include "objects.h"

constexpr int MAX_NUM_SHOTS = 5;

struct Player {
	struct Object object;
	struct Shot shots[MAX_NUM_SHOTS];
	int lives;
};

void player_init(struct Player *player);
