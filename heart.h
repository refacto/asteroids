#pragma once

#include <raylib.h>

struct Heart {
	Color color;
	Vector2 position;    // center of the square
	float scale;         // width of the primitive shapes
	Vector2 _anchors[2]; // private. Left / right circle centers
};

void heart_init(struct Heart *heart, float scale, Vector2 position,
				Color color);

void heart_set_pos(struct Heart *heart, Vector2 position);

void heart_draw(struct Heart *heart);
