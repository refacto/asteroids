#pragma once
#include "fontLoader.h"
#include <stddef.h>
#include <stdint.h>

struct Score {
	int32_t value;
	struct FontLoader *fontLoader;
};

void score_init(struct Score *score, struct FontLoader *fontLoader);

// adds points to the score (may be negative)
void score_add(struct Score *score, int32_t points);

void score_draw(const struct Score *score);

// render the string representation to buf, it always null terminates
void score_render(const struct Score *score, char buf[static 32], int size);
