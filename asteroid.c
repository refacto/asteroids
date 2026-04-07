#include "asteroid.h"
#include "objects.h"
#include <math.h>
#include <raylib.h>
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

void asteroid_move(struct Asteroid *asteroid, Vector2 screen_dimensions) {
	object_move(&asteroid->object);
	object_wrap_screen(&asteroid->object, screen_dimensions, asteroid->radius);
}

// moves the origin centered shape points to the logical location on screen,
// accounting for the object rotation
static void transformPoints(struct Asteroid *asteroid,
							Vector2 out[ASTEROID_NUM_POINTS]) {
	for (int i = 0; i < ASTEROID_NUM_POINTS; i++) {
		out[i] = object_transform_vec(&asteroid->object, asteroid->points[i]);
	}
}

void asteroid_draw(struct Asteroid *asteroid) {
	// we need to draw around the shape center, so move all points there first
	Vector2 shiftedPoints[ASTEROID_NUM_POINTS];
	transformPoints(asteroid, shiftedPoints);
	// Now connect all consecutive points with lines, this yields a jagged shape
	for (int idx = 0; idx < ASTEROID_NUM_POINTS; idx++) {
		int nextIdx = (idx + 1) % ASTEROID_NUM_POINTS;
		Vector2 thisPoint = shiftedPoints[idx];
		Vector2 nextPoint = shiftedPoints[nextIdx];
		DrawLine((int)thisPoint.x, (int)thisPoint.y, (int)nextPoint.x,
				 (int)nextPoint.y, WHITE);
	}
}
