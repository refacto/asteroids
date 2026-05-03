#include "heart.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>

static void calc_points(struct Heart *heart) {
	Vector2 position = heart->position;
	float sqrHeight = sqrtf(2) * heart->scale; // the rotated square height
	// there's probably easier ways than that monstrosity, but it works
	// we walk 1/2 the length of the square edge, that gives the midpoint
	Vector2 top = {.x = position.x, .y = position.y - sqrHeight / 2};
	Vector2 left = {.x = position.x - sqrHeight / 2, .y = position.y};
	Vector2 right = {.x = position.x + sqrHeight / 2, .y = position.y};
	heart->_anchors[0] = Vector2Scale(Vector2Add(left, top), 0.5f);
	heart->_anchors[1] = Vector2Scale(Vector2Add(right, top), 0.5f);
}

void heart_init(struct Heart *heart, float scale, Vector2 position,
				Color color) {
	*heart = (struct Heart){
		.scale = scale,
		.position = position,
		.color = color,
	};
	calc_points(heart);
}

void heart_set_pos(struct Heart *heart, Vector2 position) {
	heart->position = position;
	calc_points(heart);
}

void heart_draw(struct Heart *heart) {
	float offset = heart->scale / 2;
	DrawRectanglePro(
		(Rectangle){
			.x = heart->position.x,
			.y = heart->position.y,
			.height = heart->scale,
			.width = heart->scale,
		},
		(Vector2){
			.x = offset,
			.y = offset}, // relative to the rectangle, not to the global XY
		45, heart->color);
	DrawCircleV(heart->_anchors[0], heart->scale * 0.5f, heart->color);
	DrawCircleV(heart->_anchors[1], heart->scale * 0.5f, heart->color);
}
