#include "game.h"
#define unused [[maybe_unused]]

void game_init(struct Game *game) {
	*game = (struct Game){};
	player_init(&game->player);
}

void game_destroy(unused struct Game *game) {}

void game_update(unused struct Game *game) {}
void game_draw(unused struct Game *game) {}
