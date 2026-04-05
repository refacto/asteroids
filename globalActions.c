#include "globalActions.h"

static bool quitRequested = false;

void quit_request() {
	quitRequested = true;
}
bool quit_isRequested() {
	return quitRequested;
}
