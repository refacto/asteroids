#pragma once
#include "objects.h"

constexpr int ASTEROID_NUM_POINTS = 10;

struct Asteroid {
	struct Object object;
	float radius;
	// raw shape points, centered at the origin
	Vector2 points[ASTEROID_NUM_POINTS];
	// logical shape points, rotated and centered around the position
	Vector2 transformedPoints[ASTEROID_NUM_POINTS];
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

void asteroid_update(struct Asteroid *asteroid, Vector2 screen_dimensions);

void asteroid_set_velocity(struct Asteroid *asteroid, Vector2 velocity);

void asteroid_set_position(struct Asteroid *asteroid, Vector2 position);

bool asteroid_collide_circle(struct Asteroid *asteroid, Vector2 position,
							 float radius);

// TODO: remove, just here for demo
void asteroid_stop_moving(struct Asteroid *asteroid);
