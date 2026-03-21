#include "objects.h"

constexpr float ASTEROID_LARGE_RADIUS = 40.0f;
constexpr int PLAYER_STARTING_LIVES = 3;

void player_init(struct Player *player) {
	*player = (struct Player){.lives = PLAYER_STARTING_LIVES};
}

void asteroid_init(struct Asteroid *asteroid) {
	*asteroid = (struct Asteroid){.radius = ASTEROID_LARGE_RADIUS};
}

bool asteroid_has_next(const struct Asteroid *asteroid) {
	return asteroid->next != nullptr;
}

struct Asteroid *asteroid_next(const struct Asteroid *asteroid) {
	return asteroid->next;
}
