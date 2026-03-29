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
				.acceleration_factor = 1,
			},
	};
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
	DrawCircle((int)asteroid->object.position.x,
			   (int)asteroid->object.position.y, asteroid->radius, VIOLET);
}

void object_move(struct Object *obj) {
	Vector2 new_velocity =
		Vector2Scale(obj->velocity, obj->acceleration_factor);
	obj->velocity = Vector2ClampValue(new_velocity, 0, obj->max_velocity);
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
