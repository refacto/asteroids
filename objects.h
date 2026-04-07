#pragma once

#include <raylib.h>

struct Object {
	Vector2 position;
	Vector2 velocity;
	float thrust;
	float max_thrust;
	float max_velocity;
	Color color;
	float rotation;
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
