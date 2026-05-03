#pragma once
#include "fontLoader.h"

struct Score {
	int value;
	float noHitTimer;
	struct FontLoader *fontLoader;
};

void score_init(struct Score *score, struct FontLoader *fontLoader);
void score_update(struct Score *score);

// awarded when a shot destroys an asteroid
void score_award_asteroid_hit(struct Score *score);

// resets the no-hit bonus timer when the player is hit
void score_player_hit(struct Score *score);

void score_draw(const struct Score *score);
