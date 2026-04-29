#include "designCanvas.h"
#include "screenDimensions.h"
#include <raylib.h>

#define unused [[maybe_unused]]

void designCanvas_init(unused struct DesignCanvas *canvas) {}

void designCanvas_update(unused struct DesignCanvas *canvas) {}
void designCanvas_destroy(unused struct DesignCanvas *canvas) {}

void designCanvas_draw(unused struct DesignCanvas *canvas) {
	Vector2 screenDimensions = screenDimensions_get();
	ClearBackground(BLACK);
	Vector2 top = {.x = screenDimensions.x / 2, .y = 0};
	Vector2 bottom = {.x = screenDimensions.x / 2, .y = screenDimensions.y};
	DrawLine((int)top.x, (int)top.y, (int)bottom.x, (int)bottom.y, WHITE);
	Vector2 left = {.x = 0, .y = screenDimensions.y / 2};
	Vector2 right = {.x = screenDimensions.x, .y = screenDimensions.y / 2};
	DrawLine((int)left.x, (int)left.y, (int)right.x, (int)right.y, WHITE);
}

void designCanvas_screen_draw(void *data) {
	designCanvas_draw((struct DesignCanvas *)data);
}

void designCanvas_screen_update(unused struct ScreenController *ctrl,
								void *data) {
	designCanvas_update((struct DesignCanvas *)data);
}
