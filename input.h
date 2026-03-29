#pragma once

enum KeyAction {
	ACTION_NULL,
	ACTION_UP,
	ACTION_LEFT,
	ACTION_DOWN,
	ACTION_RIGHT,
	ACTION_EXECUTE, // select, fire etc
};

bool input_key_once(enum KeyAction action);

bool input_key_down(enum KeyAction action);
