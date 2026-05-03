#pragma once

#include "asteroidShower.h"
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
};

void scoreboard_init(struct Scoreboard *scoreboard,
					 struct FontLoader *fontLoader,
					 struct AsteroidShower *asteroidShower);

// Inserts a new entry into the top 5 if its score qualifies.
// Lower-ranked entries shift down; the lowest may fall off.
void scoreboard_add_entry(struct Scoreboard *scoreboard, const char *name,
						  int score);

void scoreboard_draw(struct Scoreboard *scoreboard);

void scoreboard_screen_update(struct ScreenController *ctrl, void *data);
void scoreboard_screen_draw(void *data);
