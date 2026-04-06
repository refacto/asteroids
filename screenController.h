#pragma once

enum ScreenType {
	SCREEN_TITLE,
	SCREEN_GAME,
	SCREEN_SCORE,
	_SCREEN_MAX, // EOL marker
};

struct ScreenController; // fwd declaration

struct Screen {
	void (*update)(struct ScreenController *ctrl, void *data);
	void (*draw)(struct ScreenController *ctrl, void *data);
	void *data; // callback data
};

struct ScreenController {
	struct Screen *active;
	struct Screen *transition;
	struct Screen screens[_SCREEN_MAX];
};

void screen_register(struct ScreenController *controller, enum ScreenType type,
					 struct Screen *screen);

void screen_transition(struct ScreenController *controller,
					   enum ScreenType type);

void screen_set_active(struct ScreenController *controller,
					   enum ScreenType type);

void screen_update(struct ScreenController *controller);

void screen_draw(struct ScreenController *controller);
