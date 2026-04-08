#include "player.h"
#include "asteroid.h"
#include "input.h"
#include "objects.h"
#include "raylib.h"
#include "shot.h"
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
	for (int i = 0; i < MAX_NUM_SHOTS; i++) {
		shot_init(&player->shots[i]);
	}
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

static struct Shot *get_inactive_shot(struct Player *player) {
	for (int i = 0; i < MAX_NUM_SHOTS; i++) {
		struct Shot *cur = &player->shots[i];
		if (!cur->active) {
			return cur;
		}
	}
	return nullptr;
}

static void try_fire_shot(struct Player *player) {
	struct Shot *shot = get_inactive_shot(player);
	if (!shot) {
		return; // exceeded the maximum amount of bullets
	}
	Vector2 startpos = shiphead_position(player);
	shot_fire(shot, startpos, player->object.rotation);
}

static void handle_input(struct Player *player) {
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
	if (input_key_once(ACTION_EXECUTE)) {
		try_fire_shot(player);
	}
}

// moves the origin centered shape points to the logical location on screen,
// accounting for the object rotation
static void transform_points(struct Player *player) {
	for (int i = 0; i < PLAYER_NUM_POINTS; i++) {
		player->transformedPoints[i] =
			object_transform_vec(&player->object, player->points[i]);
	}
}

static void update_shots(struct Player *player) {
	for (int i = 0; i < MAX_NUM_SHOTS; i++) {
		shot_update(&player->shots[i]);
	}
}

void player_update(struct Player *player) {
	handle_input(player);
	update_shots(player);
	// FIXME: this should be passed in to update
	player_move(player, (Vector2){.x = 800, .y = 450});
	transform_points(player);
}

static void draw_shots(struct Player *player) {
	for (int i = 0; i < MAX_NUM_SHOTS; i++) {
		shot_draw(&player->shots[i]);
	}
}

void player_draw(struct Player *player) {
	Vector2 *points = player->transformedPoints;
	DrawTriangleLines(points[0], points[1], points[2], player->object.color);
#ifdef DEBUG_SHIP
	DrawCircle((int)player->object.position.x, (int)player->object.position.y,
			   3, YELLOW);
	DrawCircleLines((int)player->object.position.x,
					(int)player->object.position.y, SHIP_HEIGHT / 2.0, YELLOW);
#endif
	draw_shots(player);

#ifdef DEBUG_SHIP
	render_debug(player);
#endif
}

void player_move(struct Player *player, Vector2 screen_dimensions) {
	object_move(&player->object);
	object_wrap_screen(&player->object, screen_dimensions, SHIP_HEIGHT);
}

static struct Shot *find_shot_collided(struct Player *player,
									   struct Asteroid *asteroid) {
	for (int i = 0; i < MAX_NUM_SHOTS; i++) {
		struct Shot *shot = &player->shots[i];
		if (!shot->active) {
			continue;
		}
		if (shot_collide_asteroid(shot, asteroid)) {
			return shot;
		};
	}
	return nullptr;
}

static bool check_player_collided(struct Player *player,
								  struct Asteroid *asteroid) {
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

void player_mark_shot(struct Player *player, bool shot) {
	if (shot) {
		player->object.color = RED;
		return;
	}
	player->object.color = WHITE;
}

enum CollisionResult player_check_collision(struct Player *player,
											struct Asteroid *asteroid) {
	struct Shot *shot = find_shot_collided(player, asteroid);
	if (shot) {
		shot_stop_moving(shot);
		return DESTROYED;
	}
	if (check_player_collided(player, asteroid)) {
		return PLAYER_DAMAGE;
	}
	return NO_HIT;
}
