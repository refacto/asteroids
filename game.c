#include "game.h"
#include "asteroid.h"
#include "player.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define unused [[maybe_unused]]

constexpr float SAFE_SPAWN_RADIUS = 120;

static struct Asteroid *new_asteroid(struct Game *game) {
	struct Asteroid *asteroid = malloc(sizeof(*asteroid));
	asteroid_init(asteroid);
	// somewhat unclean, but we know that the player spawns in the center.
	// so give the player some space
	Vector2 screenCenter = Vector2Scale(game->screen_dimensions, 0.5);
	Vector2 position;
	do {
		position = (Vector2){
			.x = (float)GetRandomValue(0, (int)game->screen_dimensions.x),
			.y = (float)GetRandomValue(0, (int)game->screen_dimensions.y),
		};
		asteroid_set_position(asteroid, position);
	} while (asteroid_collide_circle_coarse(asteroid, screenCenter,
											SAFE_SPAWN_RADIUS));
	return asteroid;
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
		struct Asteroid *asteroid = new_asteroid(game);
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
	struct Asteroid *asteroid = game->asteroids;
	while (asteroid) {
		asteroid_update(asteroid, game->screen_dimensions);
		asteroid = asteroid->next;
	}
	player_update(&game->player);
	asteroid = game->asteroids;
	bool player_hit = false;
	while (asteroid) {
		enum CollisionResult res =
			player_check_collision(&game->player, asteroid);
		switch (res) {
			case DESTROYED: {
				// TODO: handle scoring
				asteroid_stop_moving(asteroid);
				break;
			}
			case PLAYER_DAMAGE: {
				// TODO: handle life tracking
				player_hit = true;
				break;
			}
			case NO_HIT: {
				break;
			}
		}
		asteroid = asteroid->next;
	}
	player_mark_shot(&game->player, player_hit);
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
