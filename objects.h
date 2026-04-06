#pragma once

#include "raylib.h"

struct Object {
	Vector2 position;
	Vector2 velocity;
	float thrust;
	float max_thrust;
	float max_velocity;
	Color color;
	float rotation;
};

constexpr int ASTEROID_NUM_VERTICES = 10;

struct Asteroid {
	struct Object object;
	float radius;
	// vertex offsets as fraction of radius [0.5 - 1.0], giving jagged shape
	float vertex_offsets[ASTEROID_NUM_VERTICES];
	// intrusive linked list
	struct Asteroid *next;
};

// moves an object, adjusting its speed based on acceleration
void object_move(struct Object *obj);

// wraps an object around the screen.
// wrap_offset describes the padding that's added to avoid jumping
void object_wrap_screen(struct Object *obj, Vector2 screen_dimensions,
						float wrap_offset);

// transform a given vector into the global coordinate system
// applying rotation and position offset
Vector2 object_transform_vec(struct Object *obj, Vector2 vec);

// rotate by delta (in deg)
void object_rotate(struct Object *obj, float delta);

// add delta to thrust, capping at max_thrust
void object_thrust_inc(struct Object *obj, float delta);

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
