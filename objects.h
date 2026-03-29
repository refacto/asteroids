#pragma once

#include "raylib.h"

struct Object {
	Vector2 position;
	Vector2 velocity;
	float acceleration_factor; // [0;1]
	float max_velocity;
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

// moves an object, adjusting its speed based on acceleration
void object_move(struct Object *obj);

// wraps an object around the screen.
// wrap_offset describes the padding that's added to avoid jumping
void object_wrap_screen(struct Object *obj, Vector2 screen_dimensions,
						float wrap_offset);

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
