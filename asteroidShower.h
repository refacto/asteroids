#pragma once

#include "asteroid.h"
#include <raylib.h>

#define ASTEROID_SHOWER_COUNT 10

struct AsteroidShower {
	struct Asteroid asteroids[ASTEROID_SHOWER_COUNT];
	Vector2 screenDimensions;
};

void asteroidShower_init(struct AsteroidShower *as, Vector2 screenDimensions);
void asteroidShower_update(struct AsteroidShower *as);
void asteroidShower_draw(struct AsteroidShower *as);
