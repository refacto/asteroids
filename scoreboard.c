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
	return true;
}

static void scoreboard_commit_entry(struct Scoreboard *scoreboard) {
	struct ScoreEntry *entry = &scoreboard->entries[scoreboard->enteringIndex];
	if (entry->name[0] == '\0') {
		snprintf(entry->name, sizeof(entry->name), "PLAYER");
	}
	scoreboard->enteringIndex = -1;
	scoreboard_save(scoreboard);
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
	Vector2 nameW = MeasureTextEx(fe.font, "WWWWWWWW", fs, sp);
	float scoreW = MeasureTextEx(fe.font, "999999", fs, sp).x;
	float gap = nameW.x / 8;
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
		if (editing && blink_should_draw(&scoreboard->cursorBlink)) {
			float caretX = nameX + MeasureTextEx(fe.font, e->name, fs, sp).x;
			DrawTextEx(fe.font, "_", (Vector2){caretX, y}, fs, sp, color);
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
		blink_update(&scoreboard->cursorBlink, GetFrameTime());
		struct ScoreEntry *entry =
			&scoreboard->entries[scoreboard->enteringIndex];
		size_t len = strlen(entry->name);

		if (IsKeyPressed(KEY_BACKSPACE) && len > 0) {
			entry->name[--len] = '\0';
			blink_reset(&scoreboard->cursorBlink);
		}

		int ch;
		while ((ch = GetCharPressed()) != 0) {
			if (len >= SCOREBOARD_NAME_LEN) {
				continue;
			}

			if (ch >= 'a' && ch <= 'z')
				ch -= 'a' - 'A';

			bool isUpper = ch >= 'A' && ch <= 'Z';
			bool isDigit = ch >= '0' && ch <= '9';
			if (!isUpper && !isDigit) {
				continue;
			}
			entry->name[len++] = (char)ch;
			entry->name[len] = '\0';
			blink_reset(&scoreboard->cursorBlink);
		}

		if (IsKeyPressed(KEY_ENTER)) {
			scoreboard_commit_entry(scoreboard);
		}
		return;
	}

	if (input_key_once(ACTION_EXECUTE)) {
		screen_transition(ctrl, SCREEN_TITLE);
	}
}

void scoreboard_screen_draw(void *data) {
	scoreboard_draw((struct Scoreboard *)data);
}
