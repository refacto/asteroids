#pragma once

#include <raylib.h>
enum FontType {
	FONT_NORMAL,
	FONT_TITLE,
	_FONT_MAX, // EOL
};

struct FontEntry {
	Font font;
	int size;
	float spacing;
};

struct FontLoader {
	struct FontEntry fonts[_FONT_MAX];
};

void fontLoader_init(struct FontLoader *loader);
void fontLoader_destroy(struct FontLoader *loader);
struct FontEntry fontLoader_get(struct FontLoader *loader, enum FontType type);
