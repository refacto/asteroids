#include "game.h"
#include "asteroid.h"
#include "input.h"
#include "player.h"
#include "shot.h"
#include "soundFx.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define unused [[maybe_unused]]

static Vector2 random_location(Vector2 screen_dimensions) {
	return (Vector2){
		.x = (float)GetRandomValue(0, (int)screen_dimensions.x),
		.y = (float)GetRandomValue(0, (int)screen_dimensions.y),
	};
}

void game_init(struct Game *game, int screenWidth, int screenHeight,
			   struct SoundFx *sfx) {
	*game = (struct Game){
		.screen_dimensions =
			{
				.x = (float)screenWidth,
				.y = (float)screenHeight,
			},
		.sfx = sfx,
	};
	player_init(&game->player, game->screen_dimensions);
	struct Asteroid *last = nullptr;
	for (int i = 0; i < 10; i++) {
		struct Asteroid *asteroid = asteroid_new();
		asteroid_set_position(asteroid,
							  random_location(game->screen_dimensions));
		asteroid_set_next(asteroid, last);
		last = asteroid;
	}
	game->asteroids = last;
	for (int i = 0; i < MAX_NUM_SHOTS; i++) {
		shot_init(&game->shots[i]);
	}
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

static struct Shot *get_inactive_shot(struct Game *game) {
	for (int i = 0; i < MAX_NUM_SHOTS; i++) {
		struct Shot *cur = &game->shots[i];
		if (!cur->active) {
			return cur;
		}
	}
	return nullptr;
}

static void try_fire_shot(struct Game *game) {
	struct Shot *shot = get_inactive_shot(game);
	if (!shot) {
		return; // exceeded the maximum amount of bullets
	}
	Vector2 startpos = player_cannon_position(&game->player);
	shot_fire(shot, startpos, game->player.object.rotation);
	soundFx_play(game->sfx, SFX_SHOOT);
}

static void update_shots(struct Game *game) {
	for (int i = 0; i < MAX_NUM_SHOTS; i++) {
		shot_update(&game->shots[i]);
	}
}

static void update_asteroids(struct Game *game) {
	struct Asteroid *asteroid = game->asteroids;
	while (asteroid) {
		asteroid_update(asteroid, game->screen_dimensions);
		asteroid = asteroid->next;
	}
}

static void handle_input(struct Game *game) {
	constexpr float rotation_increment = 5;
	if (input_key_down(ACTION_LEFT)) {
		player_rotate(&game->player, -rotation_increment);
	}
	if (input_key_down(ACTION_RIGHT)) {
		player_rotate(&game->player, rotation_increment);
	}
	constexpr float thrust_inc = 0.2f;
	if (input_key_down(ACTION_UP)) {
		player_thrust_inc(&game->player, thrust_inc);
	} else {
		player_thrust_inc(&game->player, -thrust_inc / 2);
	}
	if (input_key_down(ACTION_DOWN)) {
		player_thrust_inc(&game->player, -thrust_inc);
	}
	if (input_key_once(ACTION_EXECUTE)) {
		try_fire_shot(game);
	}
}

static struct Shot *find_shot_collided(struct Game *game,
									   struct Asteroid *asteroid) {
	for (int i = 0; i < MAX_NUM_SHOTS; i++) {
		struct Shot *shot = &game->shots[i];
		if (!shot->active) {
			continue;
		}
		if (shot_collide_asteroid(shot, asteroid)) {
			return shot;
		};
	}
	return nullptr;
}

static bool collide_asteroid_shots(struct Game *game,
								   struct Asteroid *asteroid) {
	struct Shot *shot = find_shot_collided(game, asteroid);
	if (!shot) {
		return false;
	}
	// TODO: handle scoring
	Vector2 damageDirection = shot->object.velocity;
	shot_set_active(shot, false);
	if (asteroid_can_split(asteroid)) {
		struct Asteroid *asteroids = asteroid_split(asteroid, damageDirection);
		asteroid_add(game->asteroids, asteroids);
	}
	soundFx_play(game->sfx, SFX_ASTEROID_HIT);
	return true;
}

static void handle_collisions(struct Game *game) {
	bool player_hit = false;
	struct Asteroid *asteroid = game->asteroids;
	while (asteroid) {
		if (collide_asteroid_shots(game, asteroid)) {
			asteroid_remove(&game->asteroids, asteroid);
			struct Asteroid *next = asteroid->next;
			free(asteroid);
			asteroid = next;
			// don't want to check for player collision
			continue;
		}
		// this short circuits, so the player can only be hit once currently
		player_hit =
			player_hit || player_check_collision(&game->player, asteroid);
		asteroid = asteroid->next;
	}
	game->player_was_hit = player_hit;
	// TODO: handle life tracking
	if (player_hit) {
		soundFx_play(game->sfx, SFX_PLAYER_HIT);
		player_set_invincible(&game->player);
	}
	player_mark_shot(&game->player, player_hit);
}

void game_update(struct Game *game) {
	update_asteroids(game);
	player_update(&game->player);
	update_shots(game);
	handle_collisions(game);
	handle_input(game);
}

static void draw_shots(struct Game *game) {
	for (int i = 0; i < MAX_NUM_SHOTS; i++) {
		shot_draw(&game->shots[i]);
	}
}

void game_draw(struct Game *game) {
	ClearBackground(BLACK);
	struct Asteroid *cur = game->asteroids;
	while (cur) {
		asteroid_draw(cur);
		cur = cur->next;
	}
	draw_shots(game);
	player_draw(&game->player);
}

void game_screen_update(unused struct ScreenController *ctrl, void *data) {
	game_update((struct Game *)data);
}

void game_screen_draw(void *data) {
	game_draw((struct Game *)data);
}
