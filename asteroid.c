#include "asteroid.h"
#include "objects.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

constexpr float ASTEROID_LARGE_RADIUS = 40.0f;
constexpr int ASTEROID_MAX_VELOCITY = 4;

static void populate_radius_scales(float offsets[ASTEROID_NUM_POINTS]) {
	for (int i = 0; i < ASTEROID_NUM_POINTS; i++) {
		if (GetRandomValue(0, 1)) {
			offsets[i] = 1.0f;
		} else {
			offsets[i] = 0.5f + (float)GetRandomValue(0, 50) / 100.0f;
		}
	}
}

static void fill_points(Vector2 points[ASTEROID_NUM_POINTS], float radius) {
	// We spread points around a unit circle: half sit on the circle (1.0),
	// the other half are shifted inward for a jagged asteroid shape
	float radiusScaleFactors[ASTEROID_NUM_POINTS];
	populate_radius_scales(radiusScaleFactors);
	float angle_step = 360.0f / ASTEROID_NUM_POINTS;
	for (int i = 0; i < ASTEROID_NUM_POINTS; i++) {
		float alpha = (float)i * angle_step * DEG2RAD;
		points[i].x = cosf(alpha) * radius * radiusScaleFactors[i];
		points[i].y = sinf(alpha) * radius * radiusScaleFactors[i];
	}
}

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
	fill_points(asteroid->points, asteroid->radius);
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

void asteroid_set_velocity(struct Asteroid *asteroid, Vector2 velocity) {
	asteroid->object.velocity = velocity;
}

void asteroid_set_position(struct Asteroid *asteroid, Vector2 position) {
	asteroid->object.position = position;
}

void asteroid_move(struct Asteroid *asteroid, Vector2 screen_dimensions) {
	object_move(&asteroid->object);
	object_wrap_screen(&asteroid->object, screen_dimensions, asteroid->radius);
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
	return CheckCollisionCircles(asteroid->object.position, asteroid->radius,
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

// TODO: remove, just here for demo
void asteroid_stop_moving(struct Asteroid *asteroid) {
	asteroid->object.velocity = Vector2Zero();
	asteroid->object.thrust = 0;
}
