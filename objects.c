#include "objects.h"

constexpr int ASTEROID_LARGE_RADIUS = 40;
constexpr int PLAYER_STARTING_LIFES = 3;

void player_init(struct Player *player) {
	*player = (struct Player){.lifes = PLAYER_STARTING_LIFES};
}

void asteroid_init(struct Asteroid *asteroid) {
	*asteroid = (struct Asteroid){.radius = ASTEROID_LARGE_RADIUS};
}
