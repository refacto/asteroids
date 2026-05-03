#include "score.h"
#include "screenDimensions.h"
#include <raylib.h>
#include <stdio.h>

constexpr int POINTS_PER_ASTEROID = 100;
constexpr int POINTS_PER_NO_HIT_INTERVAL = 50;
constexpr float NO_HIT_INTERVAL = 10.0f;

void score_init(struct Score *score, struct FontLoader *fontLoader) {
	*score = (struct Score){
		.fontLoader = fontLoader,
	};
}

void score_update(struct Score *score) {
	score->noHitTimer += GetFrameTime();
	if (score->noHitTimer > NO_HIT_INTERVAL) {
		score->noHitTimer = 0.0f;
		score->value += POINTS_PER_NO_HIT_INTERVAL;
	}
}

void score_award_asteroid_hit(struct Score *score) {
	score->value += POINTS_PER_ASTEROID;
}

void score_player_hit(struct Score *score) {
	score->noHitTimer = 0;
}

void score_draw(const struct Score *score) {
	constexpr float padding = 10.0f;
	char buf[32];
	snprintf(buf, sizeof(buf), "Score: %d", score->value);
	struct FontEntry fontEntry = fontLoader_get(score->fontLoader, FONT_NORMAL);
	Vector2 dims = MeasureTextEx(fontEntry.font, buf, (float)fontEntry.size,
								 fontEntry.spacing);
	Vector2 pos = {
		.x = screenDimensions_get().x - dims.x - padding,
		.y = padding,
	};
	DrawTextEx(fontEntry.font, buf, pos, (float)fontEntry.size,
			   fontEntry.spacing, RAYWHITE);
}
