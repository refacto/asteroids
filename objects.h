#pragma once

#include "raylib.h"

struct Object {
	Vector2 position;
	Vector2 velocity;
	float acceleration;
	Color color;
	float rotation;
};

struct Asteroid {
	struct Object object;
	float radius;
	// intrusive linked list
	struct Asteroid *next;
};

struct Shot {
	struct Object object;
	bool active;
};

constexpr int MAX_NUM_SHOTS = 5;

struct Player {
	struct Object object;
	struct Shot shots[MAX_NUM_SHOTS];
	int lives;
};

void asteroid_init(struct Asteroid *asteroid);

void player_init(struct Player *player);

// check if asteroid list has a next element
bool asteroid_has_next(const struct Asteroid *asteroid);

// get the next asteroid element from the list
struct Asteroid *asteroid_next(const struct Asteroid *asteroid);
