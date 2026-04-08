#include "shot.h"
#include "objects.h"
#include <raylib.h>
#include <raymath.h>

constexpr int SHOT_RADIUS = 2;
constexpr float STARTING_LIFETIME = 4.5;
constexpr float SHOT_SPEED = 5;

void shot_init(struct Shot *shot) {
	*shot = (struct Shot){
		.object =
			{
				.color = WHITE,
				.max_velocity = SHOT_SPEED,
			},
		.lifetime = STARTING_LIFETIME,
	};
}
void shot_update(struct Shot *shot) {
	if (!shot->active) {
		return;
	}
	shot->lifetime -= GetFrameTime();
	if (shot->lifetime < 0) {
		shot->active = false;
		return;
	}
	// FIXME: this should be passed in to update
	shot_move(shot, (Vector2){.x = 800, .y = 450});
}

void shot_draw(struct Shot *shot) {
	if (!shot->active) {
		return;
	}
	DrawCircle((int)shot->object.position.x, (int)shot->object.position.y,
			   SHOT_RADIUS, shot->object.color);
}

void shot_move(struct Shot *shot, Vector2 screen_dimensions) {
	object_move(&shot->object);
	object_wrap_screen(&shot->object, screen_dimensions, SHOT_RADIUS * 2);
}

void shot_fire(struct Shot *shot, Vector2 position, float rotation) {
	shot->object.position = position;
	shot->object.rotation = rotation;
	shot->object.velocity = (Vector2){
		.x = SHOT_SPEED * sinf(rotation * DEG2RAD),
		.y = -SHOT_SPEED * cosf(rotation * DEG2RAD),
	};
	shot->active = true;
	shot->lifetime = STARTING_LIFETIME;
}

void shot_set_active(struct Shot *shot, bool active) {
	shot->active = active;
}

bool shot_collide_asteroid(struct Shot *shot, struct Asteroid *asteroid) {
	return asteroid_collide_circle(asteroid, shot->object.position,
								   SHOT_RADIUS);
}

// TODO: remove, just here for demo
void shot_stop_moving(struct Shot *shot) {
	shot->object.velocity = Vector2Zero();
	shot->object.thrust = 0;
	shot->lifetime = 1000000;
}
