#include "score.h"
#include "screenDimensions.h"
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>

void score_init(struct Score *score, struct FontLoader *fontLoader) {
	*score = (struct Score){
		.fontLoader = fontLoader,
	};
}

void score_add(struct Score *score, int32_t points) {
	int32_t new_score = score->value + points;
	if (new_score < 0)
		score->value = 0;
	else
		score->value = new_score;
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
