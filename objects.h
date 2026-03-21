#pragma once

#include "raylib.h"

struct Object {
	Vector2 position;
	Vector2 speed;
	float acceleration;
	Color color;
	float rotation;
};

struct Asteroid {
	struct Object object;
	float radius;
};

struct Player {
	struct Object object;
	int lifes;
};

void asteroid_init(struct Asteroid *asteroid);

void player_init(struct Player *player);
