#include "drawHelpers.h"

float draw_text_centered(struct FontLoader *fontLoader, enum FontType type,
						 const char *text, float maxWidth, float y,
						 Color color) {
	struct FontEntry fontEntry = fontLoader_get(fontLoader, type);
	Vector2 dims = MeasureTextEx(fontEntry.font, text, (float)fontEntry.size,
								 fontEntry.spacing);
	Vector2 pos = {
		.x = maxWidth / 2 - dims.x * 0.5f,
		.y = y,
	};
	DrawTextEx(fontEntry.font, text, pos, (float)fontEntry.size,
			   fontEntry.spacing, color);
	return y + dims.y;
}
