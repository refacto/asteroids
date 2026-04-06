#include "screenController.h"
#include <stdio.h>
#include <stdlib.h>

static void valid_type_or_die(enum ScreenType type) {
	if (type >= _SCREEN_MAX || type < 0) {
		fprintf(stderr, "invalid screen type %d\n", type);
		abort();
	}
}

void screen_register(struct ScreenController *controller, enum ScreenType type,
					 struct Screen *screen) {
	valid_type_or_die(type);
	controller->screens[type] = *screen;
}

void screen_set_active(struct ScreenController *controller,
					   enum ScreenType type) {
	valid_type_or_die(type);
	controller->active = &controller->screens[type];
}

void screen_transition(struct ScreenController *controller,
					   enum ScreenType type) {
	valid_type_or_die(type);
	controller->transition = &controller->screens[type];
}

void screen_update(struct ScreenController *controller) {
	controller->active->update(controller, controller->active->data);
	if (controller->transition) {
		controller->active = controller->transition;
		controller->transition = nullptr;
		// we don't update here, so that we have a frame of delay
		// if we don't do that, we'll double interpret input on the new screen
	}
}

void screen_draw(struct ScreenController *controller) {
	controller->active->draw(controller, controller->active->data);
}
