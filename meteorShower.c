#include "meteorShower.h"
#include <math.h>
#include <raylib.h>

// direction: toward bottom-left in screen coords (Y-down)
constexpr int METEOR_DIR_CENTER = 135;
constexpr int METEOR_DIR_SPREAD = 15;

void meteorShower_init(struct MeteorShower *ms, Vector2 screenDimensions) {
	ms->screenDimensions = screenDimensions;
	int deg = GetRandomValue(METEOR_DIR_CENTER - METEOR_DIR_SPREAD,
							 METEOR_DIR_CENTER + METEOR_DIR_SPREAD);
	float speed = (float)GetRandomValue(1, 3);
	Vector2 velocity = {
		.x = speed * cosf((float)deg * DEG2RAD),
		.y = speed * sinf((float)deg * DEG2RAD),
	};
	for (int i = 0; i < METEOR_SHOWER_COUNT; i++) {
		asteroid_init(&ms->meteors[i]);
		ms->meteors[i].object.velocity = velocity;
		Vector2 pos;
		bool overlaps;
		do {
			pos = (Vector2){
				.x = (float)GetRandomValue(0, (int)screenDimensions.x),
				.y = (float)GetRandomValue(0, (int)screenDimensions.y),
			};
			overlaps = false;
			for (int j = 0; j < i; j++) {
				if (CheckCollisionCircles(pos, ms->meteors[i].radius,
										  ms->meteors[j].object.position,
										  ms->meteors[j].radius)) {
					overlaps = true;
					break;
				}
			}
		} while (overlaps);
		ms->meteors[i].object.position = pos;
	}
}

void meteorShower_update(struct MeteorShower *ms) {
	for (int i = 0; i < METEOR_SHOWER_COUNT; i++) {
		asteroid_move(&ms->meteors[i], ms->screenDimensions);
	}
}

void meteorShower_draw(struct MeteorShower *ms) {
	for (int i = 0; i < METEOR_SHOWER_COUNT; i++) {
		asteroid_draw(&ms->meteors[i]);
	}
}
