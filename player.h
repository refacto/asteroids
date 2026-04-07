#pragma once
#include "asteroid.h"
#include "objects.h"
#include "shot.h"

constexpr int MAX_NUM_SHOTS = 5;

struct Player {
	struct Object object;
	struct Shot shots[MAX_NUM_SHOTS];
	int lives;
};

enum CollisionResult {
	NO_HIT,
	DESTROYED,
	PLAYER_DAMAGE,
};

void player_init(struct Player *player, Vector2 screen_dimensions);
void player_update(struct Player *player);
void player_draw(struct Player *player);
void player_move(struct Player *player, Vector2 screen_dimensions);
enum CollisionResult player_check_collision(struct Player *player,
											struct Asteroid *asteroid);
