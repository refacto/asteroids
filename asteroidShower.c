#include "asteroidShower.h"
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
		asteroid_init(&as->asteroids[i]);
		asteroid_set_velocity(&as->asteroids[i], velocity);
		Vector2 pos;
		bool overlaps;
		do {
			pos = (Vector2){
				.x = (float)GetRandomValue(0, (int)screenDimensions.x),
				.y = (float)GetRandomValue(0, (int)screenDimensions.y),
			};
			// TODO: we should use the future general collision logic here
			overlaps = false;
			for (int j = 0; j < i; j++) {
				if (CheckCollisionCircles(pos, as->asteroids[i].radius,
										  as->asteroids[j].object.position,
										  as->asteroids[j].radius)) {
					overlaps = true;
					break;
				}
			}
		} while (overlaps);
		asteroid_set_position(&as->asteroids[i], pos);
	}
}

void asteroidShower_update(struct AsteroidShower *as) {
	for (int i = 0; i < ASTEROID_SHOWER_COUNT; i++) {
		asteroid_move(&as->asteroids[i], as->screenDimensions);
	}
}

void asteroidShower_draw(struct AsteroidShower *as) {
	for (int i = 0; i < ASTEROID_SHOWER_COUNT; i++) {
		asteroid_draw(&as->asteroids[i]);
	}
}
