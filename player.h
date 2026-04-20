#pragma once
#include "asteroid.h"
#include "objects.h"

constexpr int PLAYER_NUM_POINTS = 3;

struct Player {
	struct Object object;
	// raw shape points, centered at the origin
	Vector2 points[PLAYER_NUM_POINTS];
	// logical shape points, rotated and centered around the position
	Vector2 transformedPoints[PLAYER_NUM_POINTS];
	int lives;
};

void player_init(struct Player *player, Vector2 screen_dimensions);
void player_update(struct Player *player);
void player_draw(struct Player *player);
void player_move(struct Player *player, Vector2 screen_dimensions);

void player_rotate(struct Player *player, float delta);
void player_thrust_inc(struct Player *player, float delta);

// Returns the position where shots are fired from
// only valid after update was called on player
Vector2 player_cannon_position(struct Player *player);

bool player_check_collision(struct Player *player, struct Asteroid *asteroid);

// TODO: remove, debug
void player_mark_shot(struct Player *player, bool shot);
