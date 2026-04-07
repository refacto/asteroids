#include "objects.h"
#include "raymath.h"
#include <raylib.h>

void object_move(struct Object *obj) {
	// Y axis grows downwards, so we need to negate the y coordinate
	Vector2 acceleration = {
		.x = obj->thrust * sinf(obj->rotation * DEG2RAD),
		.y = -obj->thrust * cosf(obj->rotation * DEG2RAD),
	};
	Vector2 velocity_change = Vector2Scale(acceleration, GetFrameTime());
	obj->velocity = Vector2Add(obj->velocity, velocity_change);
	obj->velocity = Vector2ClampValue(obj->velocity, 0.0f, obj->max_velocity);
	obj->position = Vector2Add(obj->position, obj->velocity);
}

void object_wrap_screen(struct Object *obj, Vector2 screen_dimensions,
						float wrap_offset) {
	Vector2 newPos = obj->position;
	if (obj->position.x > screen_dimensions.x + wrap_offset) {
		newPos.x = -wrap_offset;
	} else if (obj->position.x < 0 - wrap_offset) {
		newPos.x = screen_dimensions.x + wrap_offset;
	}
	if (obj->position.y > screen_dimensions.y + wrap_offset) {
		newPos.y = -wrap_offset;
	} else if (obj->position.y < 0 - wrap_offset) {
		newPos.y = screen_dimensions.y + wrap_offset;
	}
	obj->position = newPos;
}

Vector2 object_transform_vec(struct Object *obj, Vector2 vec) {
	Vector2 rotated = Vector2Rotate(vec, obj->rotation * DEG2RAD);
	return Vector2Add(rotated, obj->position);
}

void object_rotate(struct Object *obj, float delta) {
	float next = fmodf(obj->rotation + delta, 360);
	// make sure we stay positive, fmodf keeps the sign of the first arg
	if (next < 0) {
		next += 360;
	}
	obj->rotation = next;
}

void object_thrust_inc(struct Object *obj, float delta) {
	obj->thrust += delta;
	if (obj->thrust > obj->max_thrust) {
		obj->thrust = obj->max_thrust;
	} else if (obj->thrust < 0) {
		obj->thrust = 0;
	}
}
