#include "raylib.h"

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Asteroids");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
