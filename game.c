#include "game.h"
#include <raylib.h>
#include <stdlib.h>

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
	player_init(&game->player);
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
}

void game_draw(struct Game *game) {
	ClearBackground(RAYWHITE);
	struct Asteroid *cur = game->asteroids;
	while (cur) {
		asteroid_draw(cur);
		cur = cur->next;
	}
}
