#pragma once
#include "objects.h"

struct Shot {
	struct Object object;
	bool active;
	float lifetime;
};

void shot_init(struct Shot *shot);
void shot_fire(struct Shot *shot, Vector2 position, float rotation);
void shot_update(struct Shot *shot);
void shot_draw(struct Shot *shot);
void shot_move(struct Shot *shot, Vector2 screen_dimensions);
