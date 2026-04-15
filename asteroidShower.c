#include "asteroidShower.h"
#include "asteroid.h"
#include <math.h>
#include <raylib.h>

// direction: toward bottom-left in screen coords (Y-down)
constexpr int ASTEROID_SHOWER_DIR_CENTER = 135;
constexpr int ASTEROID_SHOWER_DIR_SPREAD = 15;

void asteroidShower_init(struct AsteroidShower *as, Vector2 screenDimensions) {
	as->screenDimensions = screenDimensions;
	int deg =
		GetRandomValue(ASTEROID_SHOWER_DIR_CENTER - ASTEROID_SHOWER_DIR_SPREAD,
					   ASTEROID_SHOWER_DIR_CENTER + ASTEROID_SHOWER_DIR_SPREAD);
	float speed = (float)GetRandomValue(1, 2);
	Vector2 velocity = {
		.x = speed * cosf((float)deg * DEG2RAD),
		.y = speed * sinf((float)deg * DEG2RAD),
	};
	for (int i = 0; i < ASTEROID_SHOWER_COUNT; i++) {
		struct Asteroid *asteroid = &as->asteroids[i];
		asteroid_init(asteroid);
		asteroid_set_velocity(asteroid, velocity);
		Vector2 pos;
		bool overlaps;
		do {
			pos = (Vector2){
				.x = (float)GetRandomValue(0, (int)screenDimensions.x),
				.y = (float)GetRandomValue(0, (int)screenDimensions.y),
			};
			asteroid_set_position(asteroid, pos);
			overlaps = false;
			for (int j = 0; j < i; j++) {
				if (asteroid_collide_asteroid(asteroid, &as->asteroids[j])) {
					overlaps = true;
					break;
				}
			}
		} while (overlaps);
	}
}

void asteroidShower_update(struct AsteroidShower *as) {
	for (int i = 0; i < ASTEROID_SHOWER_COUNT; i++) {
		asteroid_update(&as->asteroids[i], as->screenDimensions);
	}
}

void asteroidShower_draw(struct AsteroidShower *as) {
	for (int i = 0; i < ASTEROID_SHOWER_COUNT; i++) {
		asteroid_draw(&as->asteroids[i]);
	}
}
