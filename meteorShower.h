#pragma once

#include "asteroid.h"
#include <raylib.h>

#define METEOR_SHOWER_COUNT 10

struct MeteorShower {
	struct Asteroid meteors[METEOR_SHOWER_COUNT];
	Vector2 screenDimensions;
};

void meteorShower_init(struct MeteorShower *ms, Vector2 screenDimensions);
void meteorShower_update(struct MeteorShower *ms);
void meteorShower_draw(struct MeteorShower *ms);
