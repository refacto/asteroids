#include "fontLoader.h"
#include "game.h"
#include "screenController.h"

#include "raylib.h"
#include <stdlib.h>

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Asteroids");
	SetTargetFPS(60);

	struct FontLoader *fontLoader = &(struct FontLoader){};
	fontLoader_init(fontLoader);
	struct Game *game = &(struct Game){};
	game_init(game, screenWidth, screenHeight);

	struct ScreenController *screenController = &(struct ScreenController){};
	screen_register(screenController, SCREEN_GAME,
					&(struct Screen){
						.draw = game_screen_draw,
						.update = game_screen_update,
						.data = game,
					});
	screen_set_active(screenController, SCREEN_GAME);

	while (!WindowShouldClose()) {
		screen_update(screenController);

		BeginDrawing();
		screen_draw(screenController);
		EndDrawing();
	}

	game_destroy(game);
	fontLoader_destroy(fontLoader);
	CloseWindow();
	return EXIT_SUCCESS;
}
