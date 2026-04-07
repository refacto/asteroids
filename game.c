#include "game.h"
#include "player.h"
#include <raylib.h>
#include <stdlib.h>

#define unused [[maybe_unused]]

static Vector2 random_location(Vector2 screen_dimensions) {
	return (Vector2){
		.x = (float)GetRandomValue(0, (int)screen_dimensions.x),
		.y = (float)GetRandomValue(0, (int)screen_dimensions.y),
	};
}

void game_init(struct Game *game, int screenWidth, int screenHeight) {
	*game = (struct Game){
		.screen_dimensions =
			{
				.x = (float)screenWidth,
				.y = (float)screenHeight,
			},
	};
	player_init(&game->player, game->screen_dimensions);
	struct Asteroid *last = nullptr;
	for (int i = 0; i < 10; i++) {
		struct Asteroid *asteroid = malloc(sizeof(*asteroid));
		asteroid_init(asteroid);
		asteroid->object.position = random_location(game->screen_dimensions);
		asteroid_set_next(asteroid, last);
		last = asteroid;
	}
	game->asteroids = last;
}

void game_destroy(struct Game *game) {
	player_destroy(&game->player);
	struct Asteroid *last;
	struct Asteroid *cur = game->asteroids;
	while (cur) {
		last = cur;
		cur = cur->next;
		free(last);
	}
}

void game_update(struct Game *game) {
	struct Asteroid *cur = game->asteroids;
	while (cur) {
		asteroid_move(cur, game->screen_dimensions);
		cur = cur->next;
	}
	player_update(&game->player);
}

void game_draw(struct Game *game) {
	ClearBackground(BLACK);
	struct Asteroid *cur = game->asteroids;
	while (cur) {
		asteroid_draw(cur);
		cur = cur->next;
	}
	player_draw(&game->player);
}

void game_screen_update(unused struct ScreenController *ctrl, void *data) {
	game_update((struct Game *)data);
}

void game_screen_draw(unused struct ScreenController *ctrl, void *data) {
	game_draw((struct Game *)data);
}
