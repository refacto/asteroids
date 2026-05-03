#pragma once
#include "fontLoader.h"
#include <stdint.h>

struct Score {
	int32_t value;
	struct FontLoader *fontLoader;
};

void score_init(struct Score *score, struct FontLoader *fontLoader);

// adds points to the score (may be negative)
void score_add(struct Score *score, int32_t points);

void score_draw(const struct Score *score);
