#include "title.h"
#include "fontLoader.h"
#include "globalActions.h"
#include "input.h"
#include "raylib.h"
#include "screenController.h"
#include <stddef.h>

#define unused [[maybe_unused]]

struct MenuOption {
	char *text;
	void (*onSelect)(struct Title *title, struct ScreenController *ctrl);
};

static void onSelectPlay(unused struct Title *title,
						 struct ScreenController *ctrl) {
	screen_transition(ctrl, SCREEN_GAME);
}

static void onSelectScore(unused struct Title *title,
						  unused struct ScreenController *ctrl) {
	// TODO
	// screen_transition(ctrl, SCREEN_SCORE);
}

static void onSelectQuit(unused struct Title *title,
						 unused struct ScreenController *ctrl) {
	quit_request();
}

static struct MenuOption entries[] = {
	{
		.text = "Play Game",
		.onSelect = onSelectPlay,
	},
	{
		.text = "Show scores",
		.onSelect = onSelectScore,
	},
	{
		.text = "Quit game",
		.onSelect = onSelectQuit,
	},
};

static int selectedEntryIdx = 0;
constexpr int nEntries = sizeof(entries) / sizeof(*entries);

void title_init(struct Title *title, struct FontLoader *fontLoader,
				int screenWidth, int screenHeight) {
	*title = (struct Title){
		.fontLoader = fontLoader,
		.screenDimensions =
			{
				.x = (float)screenWidth,
				.y = (float)screenHeight,
			},
	};
	asteroidShower_init(&title->asteroidShower, title->screenDimensions);
}

void title_destroy(unused struct Title *title) {}

static void inc_selected(int delta) {
	selectedEntryIdx += delta;
	if (selectedEntryIdx == nEntries) {
		selectedEntryIdx = 0;
	} else if (selectedEntryIdx == -1) {
		selectedEntryIdx = nEntries - 1;
	}
}

void title_update(unused struct Title *title, struct ScreenController *ctrl) {
	asteroidShower_update(&title->asteroidShower);
	if (input_key_once(ACTION_DOWN)) {
		inc_selected(1);
	}
	if (input_key_once(ACTION_UP)) {
		inc_selected(-1);
	}
	if (input_key_once(ACTION_EXECUTE)) {
		entries[selectedEntryIdx].onSelect(title, ctrl);
		// reset so that we always start at the top when coming back
		selectedEntryIdx = 0;
		return;
	}
}

// returns the y-Axis offset
static float drawTextCentered(struct Title *title, enum FontType type,
							  char const *text, float maxWidth, float y,
							  Color color) {
	struct FontEntry fontEntry = fontLoader_get(title->fontLoader, type);
	Vector2 dims = MeasureTextEx(fontEntry.font, text, (float)fontEntry.size,
								 fontEntry.spacing);
	Vector2 centeredPos = {
		.x = maxWidth / 2 - dims.x * 0.5f,
		.y = y,
	};
	DrawTextEx(fontEntry.font, text, centeredPos, (float)fontEntry.size,
			   fontEntry.spacing, color);
	return y + dims.y;
}

void title_draw(unused struct Title *title) {
	ClearBackground(BLACK);
	asteroidShower_draw(&title->asteroidShower);
	char const *titleText = "ASTEROIDS";
	float y = 20; // padding
	y = drawTextCentered(title, FONT_TITLE, titleText,
						 title->screenDimensions.x, y, MAROON);

	y += 100; // padding logo
	for (int i = 0; i < nEntries; i++) {
		Color color;
		if (i == selectedEntryIdx) {
			color = ORANGE;
		} else {
			color = RAYWHITE;
		}
		y = drawTextCentered(title, FONT_NORMAL, entries[i].text,
							 title->screenDimensions.x, y, color);
		y += 10; // padding entries
	}
}

void title_screen_update(struct ScreenController *ctrl, void *data) {
	title_update((struct Title *)data, ctrl);
}

void title_screen_draw(unused struct ScreenController *ctrl, void *data) {
	title_draw((struct Title *)data);
}
