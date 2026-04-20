#include "player.h"
#include "asteroid.h"
#include "objects.h"
#include <raylib.h>
#include <raymath.h>
#ifdef DEBUG_SHIP
#include <stdio.h>
#endif

constexpr int PLAYER_STARTING_LIVES = 3;
constexpr float SHIP_HEIGHT = 27.47f;
constexpr float SHIP_BASE = 20.0f;

static void fill_points(Vector2 points[PLAYER_NUM_POINTS]) {
	points[0] = (Vector2){.y = -SHIP_HEIGHT / 2}; // ship head
	points[1] = (Vector2){.x = -SHIP_BASE / 2, .y = SHIP_HEIGHT / 2}; // left
	points[2] = (Vector2){.x = SHIP_BASE / 2, .y = SHIP_HEIGHT / 2};  // right
}

void player_init(struct Player *player, Vector2 screen_dimensions) {
	*player = (struct Player){
		.lives = PLAYER_STARTING_LIVES,
		.object =
			{
				.position =
					{
						.x = (float)screen_dimensions.x / 2,
						.y = (float)screen_dimensions.y / 2,
					},
				.max_velocity = 4.0f,
				.thrust = 0,
				.max_thrust = 4,
				.color = WHITE,
			},
	};
	fill_points(player->points);
}

#ifdef DEBUG_SHIP
static void render_debug(struct Player *player) {
	// debug stuff
	char buf[BUFSIZ];
	float speed = Vector2Length(player->object.velocity);
	snprintf(buf, sizeof(buf), "thrust=%f\nspeed=%f", player->object.thrust,
			 speed);
	DrawText(buf, 0, 0, 12, WHITE);
	DrawFPS(600, 0);
};
#endif

static Vector2 shiphead_position(struct Player *player) {
	return player->transformedPoints[0];
}

Vector2 player_cannon_position(struct Player *player) {
	return shiphead_position(player);
}

// moves the origin centered shape points to the logical location on screen,
// accounting for the object rotation
static void transform_points(struct Player *player) {
	for (int i = 0; i < PLAYER_NUM_POINTS; i++) {
		player->transformedPoints[i] =
			object_transform_vec(&player->object, player->points[i]);
	}
}

void player_update(struct Player *player) {
	// FIXME: this should be passed in to update
	player_move(player, (Vector2){.x = 800, .y = 450});
	transform_points(player);
}

void player_draw(struct Player *player) {
	Vector2 *points = player->transformedPoints;
	DrawTriangleLines(points[0], points[1], points[2], player->object.color);
#ifdef DEBUG_SHIP
	DrawCircle((int)player->object.position.x, (int)player->object.position.y,
			   3, YELLOW);
	DrawCircleLines((int)player->object.position.x,
					(int)player->object.position.y, SHIP_HEIGHT / 2.0, YELLOW);
	render_debug(player);
#endif
}

void player_move(struct Player *player, Vector2 screen_dimensions) {
	object_move(&player->object);
	object_wrap_screen(&player->object, screen_dimensions, SHIP_HEIGHT);
}

void player_rotate(struct Player *player, float delta) {
	object_rotate(&player->object, delta);
}

void player_thrust_inc(struct Player *player, float delta) {
	object_thrust_inc(&player->object, delta);
}

void player_mark_shot(struct Player *player, bool shot) {
	if (shot) {
		player->object.color = RED;
		return;
	}
	player->object.color = WHITE;
}

bool player_check_collision(struct Player *player, struct Asteroid *asteroid) {
	if (!asteroid_collide_circle_coarse(asteroid, player->object.position,
										SHIP_HEIGHT / 2.0)) {
		return false;
	}
	// rather expensive, might need to optimize this further
	for (int i = 0; i < PLAYER_NUM_POINTS; i++) {
		if (asteroid_collide_point(asteroid, player->transformedPoints[i])) {
			return true;
		}
	}
	return false;
}
