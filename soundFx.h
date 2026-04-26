#pragma once
#include <raylib.h>

enum Sfx {
	SFX_SHOOT,
	SFX_PLAYER_HIT,
	SFX_ASTEROID_HIT,
	_SFX_MAX, // EOL
};

struct SoundFx {
	Sound sounds[_SFX_MAX];
};

void soundFx_init(struct SoundFx *sfx);
void soundFx_destroy(struct SoundFx *sfx);
void soundFx_play(struct SoundFx *sfx, enum Sfx which);
