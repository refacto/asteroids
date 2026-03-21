#include "game.h"

#include "raylib.h"
#include <stdlib.h>

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Asteroids");
	SetTargetFPS(60);

	struct Game *game = &(struct Game){};
	game_init(game);

	while (!WindowShouldClose()) {
		game_update(game);

		BeginDrawing();
		game_draw(game);
		EndDrawing();
	}

	game_destroy(game);
	CloseWindow();
	return EXIT_SUCCESS;
}
