#include "scoreboard.h"
#include "drawHelpers.h"
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
	else if (data)
		fprintf(stderr, "scoreboard: %s size mismatch (got %d, expected %zu)\n",
				SCOREBOARD_FILE, dataSize, sizeof(scoreboard->entries));

	UnloadFileData(data);
}

void scoreboard_init(struct Scoreboard *scoreboard,
					 struct FontLoader *fontLoader,
					 struct AsteroidShower *asteroidShower) {
	*scoreboard = (struct Scoreboard){
		.fontLoader = fontLoader,
		.asteroidShower = asteroidShower,
		.enteringIndex = -1,
		.cursorBlink = {.halfPeriod = 0.4f},
		.letterCounter = 0,
	};
	scoreboard_load(scoreboard);
}

bool scoreboard_begin_entry(struct Scoreboard *scoreboard, int score) {
	int insert_at = SCOREBOARD_MAX_ENTRIES;
	for (int i = 0; i < SCOREBOARD_MAX_ENTRIES; i++) {
		if (scoreboard->entries[i].score <= score) {
			insert_at = i;
			break;
		}
	}
	if (insert_at == SCOREBOARD_MAX_ENTRIES) {
		return false; // score doesn't qualify
	}
	memmove(&scoreboard->entries[insert_at + 1],
			&scoreboard->entries[insert_at],
			(size_t)(SCOREBOARD_MAX_ENTRIES - 1 - insert_at) *
				sizeof(scoreboard->entries[0]));
	scoreboard->entries[insert_at] = (struct ScoreEntry){.score = score};
	scoreboard->enteringIndex = insert_at;
	blink_reset(&scoreboard->cursorBlink);
	scoreboard->letterCounter = 0;
	return true;
}

static void scoreboard_end_entry(struct Scoreboard *scoreboard) {
	scoreboard->enteringIndex = -1;
	scoreboard_save(scoreboard);
}

// Walk back over UTF-8 continuation bytes (10xxxxxx) to find the start of
// the codepoint that ends at pos.
static size_t utf8_prev_boundary(const char *s, size_t pos) {
	while (pos > 0) {
		pos--;
		if (((unsigned char)s[pos] & 0xC0) != 0x80)
			break;
	}
	return pos;
}

static void scoreboard_handle_input(struct Scoreboard *scoreboard) {
	blink_update(&scoreboard->cursorBlink, GetFrameTime());
	struct ScoreEntry *entry = &scoreboard->entries[scoreboard->enteringIndex];

	if (IsKeyPressed(KEY_BACKSPACE)) {
		size_t len = strlen(entry->name);
		if (len > 0) {
			entry->name[utf8_prev_boundary(entry->name, len)] = '\0';
			scoreboard->letterCounter--;
		}
	}

	int cp;
	while ((cp = GetCharPressed()) != 0) {
		int size = 0;
		const char *bytes = CodepointToUTF8(cp, &size);
		size_t len = strlen(entry->name);
		if (size <= 0 || len + (size_t)size + 1 > sizeof(entry->name)) {
			continue;
		}
		if (scoreboard->letterCounter >= SCOREBOARD_NAME_MAX_CODEPOINTS) {
			continue;
		}
		memcpy(entry->name + len, bytes, (size_t)size);
		entry->name[len + (size_t)size] = '\0';
		scoreboard->letterCounter++;
	}

	if (IsKeyPressed(KEY_ENTER) && entry->name[0] != '\0') {
		scoreboard_end_entry(scoreboard);
	}
}

static void scoreboard_draw_caret(const struct Scoreboard *scoreboard,
								  const struct ScoreEntry *entry, Font font,
								  float fs, float sp, float nameX, float y,
								  Color color) {
	if (!blink_should_draw(&scoreboard->cursorBlink))
		return;
	float caretX = nameX + MeasureTextEx(font, entry->name, fs, sp).x;
	DrawTextEx(font, "_", (Vector2){caretX, y}, fs, sp, color);
}

void scoreboard_draw(struct Scoreboard *scoreboard) {
	Vector2 screen = screenDimensions_get();
	ClearBackground(BLACK);
	asteroidShower_draw(scoreboard->asteroidShower);

	float y = 20;
	y = draw_text_centered(scoreboard->fontLoader, FONT_TITLE, "SCOREBOARD",
						   screen.x, y, MAROON);
	y += 40;

	struct FontEntry fe = fontLoader_get(scoreboard->fontLoader, FONT_NORMAL);
	float fs = (float)fe.size;
	float sp = fe.spacing;

	// Measure each column at its widest possible content so they line up
	// regardless of which characters happen to render in any given row.
	float rankW = MeasureTextEx(fe.font, "5.", fs, sp).x;
	Vector2 nameW = MeasureTextEx(fe.font, "WWWWWWWWWWWW", fs, sp);
	float scoreW = MeasureTextEx(fe.font, "999999", fs, sp).x;
	float gap = nameW.x / 12;
	float rankX = (screen.x - (rankW + nameW.x + scoreW + 2 * gap)) / 2;
	float nameX = rankX + rankW + gap;
	float scoreEndX = nameX + nameW.x + gap + scoreW;

	for (int i = 0; i < SCOREBOARD_MAX_ENTRIES; i++) {
		const struct ScoreEntry *e = &scoreboard->entries[i];
		bool editing = i == scoreboard->enteringIndex;
		bool empty = !editing && e->name[0] == '\0';
		Color color = editing ? YELLOW : (empty ? GRAY : RAYWHITE);
		char rank[256], score[256];
		snprintf(rank, sizeof(rank), "%d.", i + 1);
		if (empty) {
			snprintf(score, sizeof(score), "---");
		} else {
			snprintf(score, sizeof(score), "%d", e->score);
		}
		float scoreX = scoreEndX - MeasureTextEx(fe.font, score, fs, sp).x;

		DrawTextEx(fe.font, rank, (Vector2){rankX, y}, fs, sp, color);
		const char *nameText = empty ? "---" : e->name;
		DrawTextEx(fe.font, nameText, (Vector2){nameX, y}, fs, sp, color);
		if (editing) {
			scoreboard_draw_caret(scoreboard, e, fe.font, fs, sp, nameX, y,
								  color);
		}
		DrawTextEx(fe.font, score, (Vector2){scoreX, y}, fs, sp, color);

		y += nameW.y + 8;
	}

	y += 30;
	const char *prompt = scoreboard->enteringIndex >= 0
							 ? "Type your name, [Enter] to confirm"
							 : "Press [Enter] to return";
	draw_text_centered(scoreboard->fontLoader, FONT_NORMAL, prompt, screen.x, y,
					   GRAY);
}

void scoreboard_screen_update(struct ScreenController *ctrl, void *data) {
	struct Scoreboard *scoreboard = (struct Scoreboard *)data;
	asteroidShower_update(scoreboard->asteroidShower);

	if (scoreboard->enteringIndex >= 0) {
		scoreboard_handle_input(scoreboard);
		return;
	}

	if (input_key_once(ACTION_EXECUTE)) {
		screen_transition(ctrl, SCREEN_TITLE);
	}
}

void scoreboard_screen_draw(void *data) {
	scoreboard_draw((struct Scoreboard *)data);
}
