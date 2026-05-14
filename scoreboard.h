#pragma once

#include "asteroidShower.h"
#include "blink.h"
#include "fontLoader.h"
#include "screenController.h"

constexpr int SCOREBOARD_MAX_ENTRIES = 5;
constexpr int SCOREBOARD_NAME_LEN = 8;

struct ScoreEntry {
	char name[SCOREBOARD_NAME_LEN + 1];
	int score;
};

struct Scoreboard {
	struct ScoreEntry entries[SCOREBOARD_MAX_ENTRIES];
	struct FontLoader *fontLoader;
	struct AsteroidShower *asteroidShower;
	int enteringIndex; // -1 when not entering a name
	struct Blink cursorBlink;
};

void scoreboard_init(struct Scoreboard *scoreboard,
					 struct FontLoader *fontLoader,
					 struct AsteroidShower *asteroidShower);

// Reserves a row for a new entry if the score qualifies for the top 5,
// shifting lower-ranked entries down. The row is left with an empty name
// for the player to fill in via scoreboard_screen_update; nothing is
// persisted until that name is committed. Returns true if a row was
// reserved (and the scoreboard is now in name-entry mode).
bool scoreboard_begin_entry(struct Scoreboard *scoreboard, int score);

void scoreboard_draw(struct Scoreboard *scoreboard);

void scoreboard_screen_update(struct ScreenController *ctrl, void *data);
void scoreboard_screen_draw(void *data);
