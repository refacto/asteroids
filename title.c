#include "title.h"
#include "drawHelpers.h"
#include "globalActions.h"
#include "input.h"
#include "screenDimensions.h"
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
						  struct ScreenController *ctrl) {
	screen_transition(ctrl, SCREEN_SCORE);
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
				struct AsteroidShower *asteroidShower) {
	*title = (struct Title){
		.fontLoader = fontLoader,
		.asteroidShower = asteroidShower,
	};
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
	asteroidShower_update(title->asteroidShower);
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

void title_draw(struct Title *title) {
	Vector2 screenDimensions = screenDimensions_get();

	ClearBackground(BLACK);
	asteroidShower_draw(title->asteroidShower);

	char const *titleText = "ASTEROIDS";
	float y = 20; // padding
	y = draw_text_centered(title->fontLoader, FONT_TITLE, titleText,
						   screenDimensions.x, y, MAROON);

	y += 100; // padding logo
	for (int i = 0; i < nEntries; i++) {
		Color color;
		if (i == selectedEntryIdx) {
			color = ORANGE;
		} else {
			color = RAYWHITE;
		}
		y = draw_text_centered(title->fontLoader, FONT_NORMAL, entries[i].text,
							   screenDimensions.x, y, color);
		y += 10; // padding entries
	}
}

void title_screen_update(struct ScreenController *ctrl, void *data) {
	title_update((struct Title *)data, ctrl);
}

void title_screen_draw(void *data) {
	title_draw((struct Title *)data);
}
