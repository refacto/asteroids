#include "game.h"

#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Asteroids");
	SetTargetFPS(60);
	SetExitKey(KEY_NULL);

	bool paused = false;

	struct Game *game = &(struct Game){};
	game_init(game, screenWidth, screenHeight);

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_ESCAPE)) {
			paused = !paused;
		}

		if (!paused) {
			game_update(game);
		}

		BeginDrawing();
		game_draw(game);

		if (paused) {
			const char *text = "PAUSED";
			int fontSize = 40;
			int textWidth = MeasureText(text, fontSize);
			DrawText(text, (screenWidth - textWidth) / 2,
					 (screenHeight - fontSize) / 2, fontSize, BLACK);
		}

		EndDrawing();
	}

	game_destroy(game);
	CloseWindow();
	return EXIT_SUCCESS;
}
