#include "player.h"
#include "input.h"
#include "raylib.h"

#define unused [[maybe_unused]]

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
			},
	};
}
void player_update(unused struct Player *player) {
	if (input_key_down(ACTION_LEFT)) {
		object_rotate(&player->object, -1);
	}
	if (input_key_down(ACTION_RIGHT)) {
		object_rotate(&player->object, 1);
	}
}
void player_draw(unused struct Player *player) {
	Vector2 head =
		object_transform_vec(&player->object, (Vector2){.y = -SHIP_HEIGHT / 2});
	Vector2 left = object_transform_vec(
		&player->object, (Vector2){.x = -SHIP_BASE / 2, .y = SHIP_HEIGHT / 2});
	Vector2 right = object_transform_vec(
		&player->object, (Vector2){.x = SHIP_BASE / 2, .y = SHIP_HEIGHT / 2});
	DrawTriangleLines(head, left, right, DARKBLUE);
	DrawCircle((int)player->object.position.x, (int)player->object.position.y,
			   3, BLUE);
}
void player_move(unused struct Player *player,
				 unused Vector2 screen_dimensions) {}
