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
	while (controller->transition) {
		// we need to initialize the new component, which theoretically could
		// transition yet again, so we play that dance until they stop
		controller->active = controller->transition;
		controller->transition = nullptr;
		controller->active->update(controller, controller->active->data);
	}
}

void screen_draw(struct ScreenController *controller) {
	controller->active->draw(controller, controller->active->data);
}
