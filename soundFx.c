#include "soundFx.h"
#include <raylib.h>
#include <stddef.h>

static const unsigned char SHOOT_WAV[] = {
#embed "resources/audio/shoot.wav"
};
static const unsigned char PLAYER_HIT_WAV[] = {
#embed "resources/audio/player_hit.wav"
};
static const unsigned char ASTEROID_HIT_WAV[] = {
#embed "resources/audio/asteroid_hit.wav"
};

static Sound load_embedded_sound(const unsigned char *data, size_t size) {
	Wave wave = LoadWaveFromMemory(".wav", data, (int)size);
	Sound sound = LoadSoundFromWave(wave);
	UnloadWave(wave);
	return sound;
}

void soundFx_init(struct SoundFx *sfx) {
	sfx->sounds[SFX_SHOOT] = load_embedded_sound(SHOOT_WAV, sizeof(SHOOT_WAV));
	sfx->sounds[SFX_PLAYER_HIT] =
		load_embedded_sound(PLAYER_HIT_WAV, sizeof(PLAYER_HIT_WAV));
	sfx->sounds[SFX_ASTEROID_HIT] =
		load_embedded_sound(ASTEROID_HIT_WAV, sizeof(ASTEROID_HIT_WAV));
}

void soundFx_destroy(struct SoundFx *sfx) {
	for (int i = 0; i < _SFX_MAX; i++) {
		UnloadSound(sfx->sounds[i]);
	}
}

void soundFx_play(struct SoundFx *sfx, enum Sfx which) {
	PlaySound(sfx->sounds[which]);
}
