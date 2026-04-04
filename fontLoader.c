#include "fontLoader.h"
#include <stddef.h>
#include <stdlib.h>

constexpr unsigned char audiowideTTF[] = {
#embed "resources/fonts/audiowide/Audiowide-Regular.ttf"
};
static_assert((sizeof audiowideTTF) >= 4,
			  "There should be at least 4 elements in this array.");
constexpr size_t audiowideSize = sizeof(audiowideTTF);

static Font load_audiowide(int size) {
	return LoadFontFromMemory(".ttf", audiowideTTF, audiowideSize, size,
							  nullptr, 0);
}

void fontLoader_init(struct FontLoader *loader) {
	constexpr int LARGE_FONT_SIZE = 64;
	constexpr int NORMAL_FONT_SIZE = 26;
	*loader = (struct FontLoader){};
	loader->fonts[FONT_NORMAL] = (struct FontEntry){
		.font = load_audiowide(NORMAL_FONT_SIZE),
		.size = NORMAL_FONT_SIZE,
		.spacing = 1.0f,
	};
	loader->fonts[FONT_TITLE] = (struct FontEntry){
		.font = load_audiowide(LARGE_FONT_SIZE),
		.size = LARGE_FONT_SIZE,
		.spacing = 1.0f,
	};
}

void fontLoader_destroy(struct FontLoader *loader) {
	for (int idx = 0; idx < _FONT_MAX; idx++) {
		UnloadFont(loader->fonts[idx].font);
	}
}

struct FontEntry fontLoader_get(struct FontLoader *loader, enum FontType type) {
	if (type < 0 || type >= _FONT_MAX) {
		abort();
	}
	return loader->fonts[type];
}
