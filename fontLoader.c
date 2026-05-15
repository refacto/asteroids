#include "fontLoader.h"
#include <stddef.h>
#include <stdlib.h>

constexpr unsigned char audiowideTTF[] = {
#embed "resources/fonts/audiowide/Audiowide-Regular.ttf"
};
static_assert((sizeof audiowideTTF) >= 4,
			  "There should be at least 4 elements in this array.");
constexpr size_t audiowideSize = sizeof(audiowideTTF);

// Basic ASCII (32-126) + Latin-1 Supplement (160-255) so common accented
// characters have glyphs in the atlas instead of rendering as "?".
constexpr int LATIN_CODEPOINT_COUNT = (126 - 32 + 1) + (255 - 160 + 1);

static Font load_audiowide(int size) {
	int cps[LATIN_CODEPOINT_COUNT];
	int idx = 0;
	for (int c = 32; c <= 126; c++)
		cps[idx++] = c;
	for (int c = 160; c <= 255; c++)
		cps[idx++] = c;
	return LoadFontFromMemory(".ttf", audiowideTTF, audiowideSize, size, cps,
							  LATIN_CODEPOINT_COUNT);
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
