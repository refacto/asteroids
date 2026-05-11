#pragma once

#include "fontLoader.h"
#include <raylib.h>

// Draws `text` horizontally centered within [0, maxWidth] at vertical
// position `y`, using the named font type. Returns y advanced by the
// text's height so callers can chain calls for a flowing layout.
float draw_text_centered(struct FontLoader *fontLoader, enum FontType type,
						 const char *text, float maxWidth, float y,
						 Color color);
