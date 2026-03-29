#include "player.h"

constexpr int PLAYER_STARTING_LIVES = 3;

void player_init(struct Player *player) {
	*player = (struct Player){.lives = PLAYER_STARTING_LIVES};
}
