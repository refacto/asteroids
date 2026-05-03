#include "fontLoader.h"
#include "game.h"
#include "globalActions.h"
#include "screenController.h"
#include "screenDimensions.h"
#include "soundFx.h"
#include "title.h"

#include <raylib.h>
#include <stdlib.h>

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Asteroids");
	InitAudioDevice();
	SetTargetFPS(60);

	screenDimensions_set(screenWidth, screenHeight);

	struct FontLoader *fontLoader = &(struct FontLoader){};
	fontLoader_init(fontLoader);
	struct SoundFx *sfx = &(struct SoundFx){};
	soundFx_init(sfx);
	struct Game *game = &(struct Game){};
	game_init(game, sfx, fontLoader);
	struct Title *title = &(struct Title){};
	title_init(title, fontLoader);

	struct ScreenController *screenController = &(struct ScreenController){};
	screen_register(screenController, SCREEN_TITLE,
					&(struct Screen){
						.draw = title_screen_draw,
						.update = title_screen_update,
						.data = title,
					});
	screen_register(screenController, SCREEN_GAME,
					&(struct Screen){
						.draw = game_screen_draw,
						.update = game_screen_update,
						.data = game,
					});
	screen_set_active(screenController, SCREEN_TITLE);

	while ((!WindowShouldClose()) && !quit_isRequested()) {
		screen_update(screenController);

		BeginDrawing();
		screen_draw(screenController);
		EndDrawing();
	}

	title_destroy(title);
	game_destroy(game);
	soundFx_destroy(sfx);
	fontLoader_destroy(fontLoader);
	CloseAudioDevice();
	CloseWindow();
	return EXIT_SUCCESS;
}
