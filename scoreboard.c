#include "scoreboard.h"
#include "input.h"
#include "screenDimensions.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>

static const char *SCOREBOARD_FILE = "scores.dat";

static void scoreboard_save(const struct Scoreboard *scoreboard) {
	SaveFileData(SCOREBOARD_FILE, (void *)scoreboard->entries,
				 sizeof(scoreboard->entries));
}

static void scoreboard_load(struct Scoreboard *scoreboard) {
	if (!FileExists(SCOREBOARD_FILE))
		return;

	int dataSize = 0;
	unsigned char *data = LoadFileData(SCOREBOARD_FILE, &dataSize);
	if (data && (size_t)dataSize == sizeof(scoreboard->entries))
		memcpy(scoreboard->entries, data, sizeof(scoreboard->entries));

	UnloadFileData(data);
}

void scoreboard_init(struct Scoreboard *scoreboard,
					 struct FontLoader *fontLoader,
					 struct AsteroidShower *asteroidShower) {
	*scoreboard = (struct Scoreboard){
		.fontLoader = fontLoader,
		.asteroidShower = asteroidShower,
	};
	scoreboard_load(scoreboard);
}

void scoreboard_add_entry(struct Scoreboard *scoreboard, const char *name,
						  int score) {
	for (int i = 0; i < SCOREBOARD_MAX_ENTRIES; i++) {
		if (scoreboard->entries[i].score <= score) {
			// Now shift everything smaller/equal down by one, we walk backwards
			for (int j = SCOREBOARD_MAX_ENTRIES - 1; j > i; j--) {
				scoreboard->entries[j] = scoreboard->entries[j - 1];
			}
			snprintf(scoreboard->entries[i].name,
					 sizeof(scoreboard->entries[i].name), "%.*s",
					 SCOREBOARD_NAME_LEN, name);
			scoreboard->entries[i].score = score;
			scoreboard_save(scoreboard);
			return;
		}
	}
}

static float draw_text_centered(struct Scoreboard *scoreboard,
								enum FontType type, const char *text,
								float maxWidth, float y, Color color) {
	struct FontEntry fontEntry = fontLoader_get(scoreboard->fontLoader, type);
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

void scoreboard_draw(struct Scoreboard *scoreboard) {
	Vector2 screen = screenDimensions_get();
	ClearBackground(BLACK);
	asteroidShower_draw(scoreboard->asteroidShower);

	float y = 20;
	y = draw_text_centered(scoreboard, FONT_TITLE, "SCOREBOARD", screen.x, y,
						   MAROON);
	y += 40;

	struct FontEntry fe = fontLoader_get(scoreboard->fontLoader, FONT_NORMAL);
	float fs = (float)fe.size;
	float sp = fe.spacing;

	// Measure each column at its widest possible content so they line up
	// regardless of which characters happen to render in any given row.
	float rankW = MeasureTextEx(fe.font, "5.", fs, sp).x;
	Vector2 nameW = MeasureTextEx(fe.font, "WWWWWWWW", fs, sp);
	float scoreW = MeasureTextEx(fe.font, "999999", fs, sp).x;
	float gap = nameW.x / 8;
	float rankX = (screen.x - (rankW + nameW.x + scoreW + 2 * gap)) / 2;
	float nameX = rankX + rankW + gap;
	float scoreEndX = nameX + nameW.x + gap + scoreW;

	for (int i = 0; i < SCOREBOARD_MAX_ENTRIES; i++) {
		const struct ScoreEntry *e = &scoreboard->entries[i];
		bool empty = e->name[0] == '\0';
		Color color = empty ? GRAY : RAYWHITE;
		char rank[8], score[16];
		snprintf(rank, sizeof(rank), "%d.", i + 1);
		if (empty) {
			snprintf(score, sizeof(score), "---");
		} else {
			snprintf(score, sizeof(score), "%d", e->score);
		}
		float scoreX = scoreEndX - MeasureTextEx(fe.font, score, fs, sp).x;

		DrawTextEx(fe.font, rank, (Vector2){rankX, y}, fs, sp, color);
		DrawTextEx(fe.font, empty ? "---" : e->name, (Vector2){nameX, y}, fs,
				   sp, color);
		DrawTextEx(fe.font, score, (Vector2){scoreX, y}, fs, sp, color);

		y += nameW.y + 8;
	}

	y += 30;
	draw_text_centered(scoreboard, FONT_NORMAL, "Press [Enter] to return",
					   screen.x, y, GRAY);
}

void scoreboard_screen_update(struct ScreenController *ctrl, void *data) {
	struct Scoreboard *scoreboard = (struct Scoreboard *)data;
	asteroidShower_update(scoreboard->asteroidShower);
	if (input_key_once(ACTION_EXECUTE)) {
		screen_transition(ctrl, SCREEN_TITLE);
	}
}

void scoreboard_screen_draw(void *data) {
	scoreboard_draw((struct Scoreboard *)data);
}
