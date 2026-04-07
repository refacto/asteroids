#pragma once
#include "asteroid.h"
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
bool shot_collide_asteroid(struct Shot *shot, struct Asteroid *asteroid);

// TODO: remove, just here for demo
void shot_stop_moving(struct Shot *shot);
