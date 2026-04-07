#include "input.h"
#include <raylib.h>

static bool input_key_f(enum KeyAction action, bool (*f)(int)) {
	switch (action) {
		case ACTION_NULL:
			return false;
		case ACTION_UP: {
			return f(KEY_UP) || f(KEY_W);
		}
		case ACTION_LEFT: {
			return f(KEY_LEFT) || f(KEY_A);
		}
		case ACTION_DOWN: {
			return f(KEY_DOWN) || f(KEY_S);
		}
		case ACTION_RIGHT: {
			return f(KEY_RIGHT) || f(KEY_D);
		}
		case ACTION_EXECUTE: {
			return f(KEY_SPACE) || f(KEY_ENTER);
		}
	}
}

bool input_key_once(enum KeyAction action) {
	return input_key_f(action, IsKeyPressed);
}

bool input_key_down(enum KeyAction action) {
	return input_key_f(action, IsKeyDown);
}
