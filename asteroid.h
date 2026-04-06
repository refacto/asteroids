#pragma once
#include "objects.h"

constexpr int ASTEROID_NUM_VERTICES = 10;

struct Asteroid {
	struct Object object;
	float radius;
	// vertex offsets as fraction of radius [0.5 - 1.0], giving jagged shape
	float vertex_offsets[ASTEROID_NUM_VERTICES];
	// intrusive linked list
	struct Asteroid *next;
};

void asteroid_init(struct Asteroid *asteroid);

// check if asteroid list has a next element
bool asteroid_has_next(const struct Asteroid *asteroid);

// add new to end of asteroid list, rooted at head
void asteroid_add(struct Asteroid *head, struct Asteroid *new);

// get the next asteroid element from the list
struct Asteroid *asteroid_next(const struct Asteroid *asteroid);

void asteroid_set_next(struct Asteroid *asteroid, struct Asteroid *next);

// draws the asteroid
void asteroid_draw(struct Asteroid *asteroid);

void asteroid_move(struct Asteroid *asteroid, Vector2 screen_dimensions);
