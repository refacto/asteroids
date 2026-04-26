#include "asteroid.h"
#include "malloc.h"
#include "objects.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

struct AsteroidSpec {
	int numPoints;
	float radius;
	float speed;
};

struct AsteroidSpec specs[_SIZE_MAX] = {
	[SIZE_SMALL] =
		{
			.numPoints = 10,
			.radius = 20.0f,
			.speed = 2,
		},
	[SIZE_MEDIUM] =
		{
			.numPoints = 10,
			.radius = 30.0f,
			.speed = 1,
		},
	[SIZE_LARGE] =
		{
			.numPoints = 10,
			.radius = 40.0f,
			.speed = 0.5,
		},
};

static void populate_radius_scales(int numPoints, float offsets[]) {
	for (int i = 0; i < numPoints; i++) {
		if (GetRandomValue(0, 1)) {
			offsets[i] = 1.0f;
		} else {
			offsets[i] = 0.5f + (float)GetRandomValue(0, 50) / 100.0f;
		}
	}
}

static void fill_points(int numPoints, Vector2 points[], float radius) {
	// We spread points around a unit circle: half sit on the circle (1.0),
	// the other half are shifted inward for a jagged asteroid shape
	float radiusScaleFactors[ASTEROID_MAX_NUM_POINTS];
	populate_radius_scales(numPoints, radiusScaleFactors);
	float angle_step = 360.0f / (float)numPoints;
	for (int i = 0; i < numPoints; i++) {
		float alpha = (float)i * angle_step * DEG2RAD;
		points[i].x = cosf(alpha) * radius * radiusScaleFactors[i];
		points[i].y = sinf(alpha) * radius * radiusScaleFactors[i];
	}
}

static Vector2 random_velocity(float speed) {
	int deg = GetRandomValue(0, 360);
	return (Vector2){
		.x = (float)(speed * cos((float)deg * DEG2RAD)),
		.y = (float)(speed * sin((float)deg * DEG2RAD)),
	};
}

static void asteroid_init_sized(struct Asteroid *asteroid,
								enum AsteroidSize size) {
	struct AsteroidSpec spec = specs[size];
	*asteroid = (struct Asteroid){
		.size = size,
		.object =
			{
				.velocity = random_velocity(spec.speed),
				.max_velocity = spec.speed,
				.thrust = 0,
			},
	};
	fill_points(spec.numPoints, asteroid->points, spec.radius);
}

void asteroid_init(struct Asteroid *asteroid) {
	asteroid_init_sized(asteroid, SIZE_LARGE);
}

struct Asteroid *asteroid_new() {
	struct Asteroid *asteroid = xmalloc(sizeof(*asteroid));
	asteroid_init(asteroid);
	return asteroid;
}

static struct Asteroid *asteroid_new_sized(enum AsteroidSize size) {
	struct Asteroid *asteroid = xmalloc(sizeof(*asteroid));
	asteroid_init_sized(asteroid, size);
	return asteroid;
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

void asteroid_remove(struct Asteroid **head, struct Asteroid *target) {
	if (!head) {
		return; // bad input
	}
	while (*head && *head != target) {
		head = &(*head)->next;
	}
	if (!*head) {
		return; // item wasn't on the list
	}
	*head = (*head)->next;
}

void asteroid_set_next(struct Asteroid *asteroid, struct Asteroid *next) {
	asteroid->next = next;
}

void asteroid_set_velocity(struct Asteroid *asteroid, Vector2 velocity) {
	object_set_velocity(&asteroid->object, velocity);
}

void asteroid_set_position(struct Asteroid *asteroid, Vector2 position) {
	asteroid->object.position = position;
}

bool asteroid_can_split(const struct Asteroid *asteroid) {
	return asteroid->size > SIZE_SMALL;
}

static Vector2 perpendicular_norm_vector(Vector2 original) {
	return Vector2Normalize((Vector2){
		.x = original.y,
		.y = -original.x,
	});
}

struct Asteroid *asteroid_split(const struct Asteroid *asteroid,
								Vector2 damageDirection) {
	if (asteroid->size == SIZE_SMALL) {
		return nullptr;
	}
	enum AsteroidSize newSize = asteroid->size - 1;
	struct Asteroid *left = asteroid_new_sized(newSize);
	struct Asteroid *right = asteroid_new_sized(newSize);
	left->object.position = asteroid->object.position;
	right->object.position = asteroid->object.position;
	// TODO add some randomization
	Vector2 newVelocity = perpendicular_norm_vector(damageDirection);
	float newSpeed = specs[newSize].speed;
	asteroid_set_velocity(left, Vector2Scale(newVelocity, newSpeed));
	asteroid_set_velocity(right, Vector2Scale(newVelocity, -newSpeed));
	asteroid_set_next(left, right);
	return left;
}

void asteroid_move(struct Asteroid *asteroid, Vector2 screen_dimensions) {
	object_move(&asteroid->object);
	struct AsteroidSpec spec = specs[asteroid->size];
	object_wrap_screen(&asteroid->object, screen_dimensions, spec.radius);
}

// moves the origin centered shape points to the logical location on screen,
// accounting for the object rotation
static void transform_points(struct Asteroid *asteroid) {
	for (int i = 0; i < ASTEROID_NUM_POINTS; i++) {
		asteroid->transformedPoints[i] =
			object_transform_vec(&asteroid->object, asteroid->points[i]);
	}
}

void asteroid_update(struct Asteroid *asteroid, Vector2 screen_dimensions) {
	asteroid_move(asteroid, screen_dimensions);
	// cache the transformed points, both drawing and collision need it
	transform_points(asteroid);
}

void asteroid_draw(struct Asteroid *asteroid) {
	// connect all consecutive points with lines, this yields a jagged shape
	for (int idx = 0; idx < ASTEROID_NUM_POINTS; idx++) {
		int nextIdx = (idx + 1) % ASTEROID_NUM_POINTS;
		Vector2 thisPoint = asteroid->transformedPoints[idx];
		Vector2 nextPoint = asteroid->transformedPoints[nextIdx];
		DrawLine((int)thisPoint.x, (int)thisPoint.y, (int)nextPoint.x,
				 (int)nextPoint.y, WHITE);
	}
}

bool asteroid_collide_circle_coarse(struct Asteroid *asteroid, Vector2 position,
									float radius) {
	struct AsteroidSpec spec = specs[asteroid->size];
	return CheckCollisionCircles(asteroid->object.position, spec.radius,
								 position, radius);
}

bool asteroid_collide_point(struct Asteroid *asteroid, Vector2 point) {
	return CheckCollisionPointPoly(point, asteroid->transformedPoints,
								   ASTEROID_NUM_POINTS);
}

bool asteroid_collide_circle(struct Asteroid *asteroid, Vector2 position,
							 float radius) {
	// if we aren't even in the mesh, we can't possibly collide
	if (!asteroid_collide_circle_coarse(asteroid, position, radius)) {
		return false;
	}
	// this is fairly expensive
	// we punt the radius, using the center will presumably work just fine
	// TODO: figure out closest point if that assumption doesn't pan out
	return asteroid_collide_point(asteroid, position);
}

bool asteroid_collide_asteroid(struct Asteroid *self, struct Asteroid *other) {
	// this should be good enough but...
	// TODO: collision within mesh
	struct AsteroidSpec selfSpec = specs[self->size];
	struct AsteroidSpec otherSpec = specs[other->size];
	return CheckCollisionCircles(self->object.position, selfSpec.radius,
								 other->object.position, otherSpec.radius);
}

// TODO: remove, just here for demo
void asteroid_stop_moving(struct Asteroid *asteroid) {
	asteroid->object.velocity = Vector2Zero();
	asteroid->object.thrust = 0;
}
