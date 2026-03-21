#include "objects.h"

constexpr int ASTEROID_LARGE_RADIUS = 40;

void player_init(struct Player *player) {
	*player = (struct Player){.lifes = 0};
}

void asteroid_init(struct Asteroid *asteroid) {
	*asteroid = (struct Asteroid){.radius = ASTEROID_LARGE_RADIUS};
}
