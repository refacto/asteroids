#include "player.h"
#include "input.h"
#include "objects.h"
#include "raylib.h"
#include <raymath.h>
#include <stdio.h>

constexpr int PLAYER_STARTING_LIVES = 3;
constexpr float SHIP_HEIGHT = 27.47f;
constexpr float SHIP_BASE = 20.0f;

void player_init(struct Player *player) {
	const int screenWidth = 800;
	const int screenHeight = 450;
	*player = (struct Player){
		.lives = PLAYER_STARTING_LIVES,
		.object =
			{
				.position =
					{
						.x = (float)screenWidth / 2,
						.y = (float)screenHeight / 2,
					},
				.max_velocity = 4.0f,
				.thrust = 0,
				.max_thrust = 4,
			},
	};
}

static void render_debug(struct Player *player) {
	// debug stuff
	char buf[BUFSIZ];
	float speed = Vector2Length(player->object.velocity);
	snprintf(buf, sizeof(buf), "thrust=%f\nspeed=%f", player->object.thrust,
			 speed);
	DrawText(buf, 0, 0, 12, BLACK);
};

void player_update(struct Player *player) {
	constexpr float rotation_increment = 5;
	if (input_key_down(ACTION_LEFT)) {
		object_rotate(&player->object, -rotation_increment);
	}
	if (input_key_down(ACTION_RIGHT)) {
		object_rotate(&player->object, rotation_increment);
	}
	constexpr float thrust_inc = 0.2f;
	if (input_key_down(ACTION_UP)) {
		object_thrust_inc(&player->object, thrust_inc);
	} else {
		object_thrust_inc(&player->object, -thrust_inc / 2);
	}
	if (input_key_down(ACTION_DOWN)) {
		object_thrust_inc(&player->object, -thrust_inc);
	}
	player_move(player, (Vector2){.x = 800, .y = 450});
}

void player_draw(struct Player *player) {
	Vector2 head =
		object_transform_vec(&player->object, (Vector2){.y = -SHIP_HEIGHT / 2});
	Vector2 left = object_transform_vec(
		&player->object, (Vector2){.x = -SHIP_BASE / 2, .y = SHIP_HEIGHT / 2});
	Vector2 right = object_transform_vec(
		&player->object, (Vector2){.x = SHIP_BASE / 2, .y = SHIP_HEIGHT / 2});
	DrawTriangleLines(head, left, right, DARKBLUE);
	DrawCircle((int)player->object.position.x, (int)player->object.position.y,
			   3, BLUE);

	// TODO: remove / gate behind flag
	render_debug(player);
}

void player_move(struct Player *player, Vector2 screen_dimensions) {
	object_move(&player->object);
	object_wrap_screen(&player->object, screen_dimensions, SHIP_HEIGHT);
}
