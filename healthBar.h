#pragma once

#include "heart.h"
#include <raylib.h>

constexpr int MAX_LIVES = 10;

struct HealthBar {
	int numLives;
	Vector2 position;
	struct Heart hearts[MAX_LIVES];
};

void healthBar_init(struct HealthBar *bar, Vector2 position, int numLives);
void healthBar_set_num_lifes(struct HealthBar *bar, int numLives);
void healthBar_draw(struct HealthBar *bar);
