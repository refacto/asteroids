#include "objects.h"
#include "raymath.h"
#include <math.h>
#include <raylib.h>
#include <stdio.h>

constexpr float ASTEROID_LARGE_RADIUS = 40.0f;
constexpr int ASTEROID_MAX_VELOCITY = 4;

static Vector2 random_velocity(int speed) {
	int deg = GetRandomValue(0, 360);
	return (Vector2){
		.x = (float)(speed * cos((float)deg * DEG2RAD)),
		.y = (float)(speed * sin((float)deg * DEG2RAD)),
	};
}

void asteroid_init(struct Asteroid *asteroid) {
	*asteroid = (struct Asteroid){
		.radius = ASTEROID_LARGE_RADIUS,
		.object =
			{
				.velocity = random_velocity(1),
				.max_velocity = ASTEROID_MAX_VELOCITY,
				.thrust = 0,
			},
	};
	// generate vertex offsets: most sit on the circle (1.0),
	// some are shifted inward for a jagged asteroid shape
	for (int i = 0; i < ASTEROID_NUM_VERTICES; i++) {
		if (GetRandomValue(0, 1)) {
			asteroid->vertex_offsets[i] = 1.0f;
		} else {
			asteroid->vertex_offsets[i] =
				0.5f + (float)GetRandomValue(0, 50) / 100.0f;
		}
	}
}

bool asteroid_has_next(const struct Asteroid *asteroid) {
	return asteroid && asteroid->next != nullptr;
}

struct Asteroid *asteroid_next(const struct Asteroid *asteroid) {
	if (!asteroid) {
		return nullptr;
	}
	return asteroid->next;
}

void asteroid_add(struct Asteroid *head, struct Asteroid *new) {
	if (!head) {
		fprintf(stderr, "invalid asteroid head: nullptr\n");
		return;
	}
	while (asteroid_has_next(head)) {
		head = asteroid_next(head);
	}
	head->next = new;
}

void asteroid_set_next(struct Asteroid *asteroid, struct Asteroid *next) {
	asteroid->next = next;
}

void asteroid_move(struct Asteroid *asteroid, Vector2 screen_dimensions) {
	object_move(&asteroid->object);
	object_wrap_screen(&asteroid->object, screen_dimensions, asteroid->radius);
}

void asteroid_draw(struct Asteroid *asteroid) {
	float cx = asteroid->object.position.x;
	float cy = asteroid->object.position.y;
	float r = asteroid->radius;
	float angle_step = 360.0f / ASTEROID_NUM_VERTICES;

	for (int i = 0; i < ASTEROID_NUM_VERTICES; i++) {
		int next = (i + 1) % ASTEROID_NUM_VERTICES;
		float angle1 = (float)i * angle_step * DEG2RAD;
		float angle2 = (float)next * angle_step * DEG2RAD;
		float r1 = r * asteroid->vertex_offsets[i];
		float r2 = r * asteroid->vertex_offsets[next];
		DrawLine((int)(cx + cosf(angle1) * r1), (int)(cy + sinf(angle1) * r1),
				 (int)(cx + cosf(angle2) * r2), (int)(cy + sinf(angle2) * r2),
				 WHITE);
	}
}

void object_move(struct Object *obj) {
	// Y axis grows downwards, so we need to negate the y coordinate
	Vector2 acceleration = {
		.x = obj->thrust * (float)sin(obj->rotation * DEG2RAD),
		.y = -obj->thrust * (float)cos(obj->rotation * DEG2RAD),
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
