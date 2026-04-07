#include "meteorShower.h"
#include <math.h>
#include <raylib.h>

// direction: toward bottom-left in screen coords (Y-down)
constexpr int METEOR_DIR_CENTER = 135;
// spread: +-15
constexpr int METEOR_DIR_SPREAD = 15;

static Vector2 meteor_velocity(void) {
	int deg = GetRandomValue(METEOR_DIR_CENTER - METEOR_DIR_SPREAD,
							 METEOR_DIR_CENTER + METEOR_DIR_SPREAD);
	float speed = (float)GetRandomValue(1, 3);
	return (Vector2){
		.x = speed * cosf((float)deg * DEG2RAD),
		.y = speed * sinf((float)deg * DEG2RAD),
	};
}

// spawn at a random position along the top or right edge
static Vector2 meteor_spawn_position(Vector2 screen) {
	if (GetRandomValue(0, 1)) {
		return (Vector2){
			.x = (float)GetRandomValue(0, (int)screen.x),
			.y = (float)GetRandomValue(-80, -10),
		};
	} else {
		return (Vector2){
			.x = (float)GetRandomValue((int)screen.x, (int)screen.x + 80),
			.y = (float)GetRandomValue(0, (int)screen.y),
		};
	}
}

static void meteor_init_one(struct Asteroid *m, Vector2 screen) {
	asteroid_init(m);
	m->object.position = meteor_spawn_position(screen);
	m->object.velocity = meteor_velocity();
}

void meteorShower_init(struct MeteorShower *ms, Vector2 screenDimensions) {
	ms->screenDimensions = screenDimensions;
	for (int i = 0; i < METEOR_SHOWER_COUNT; i++) {
		meteor_init_one(&ms->meteors[i], screenDimensions);
		// Spread initial positions across the screen instead of all at edges
		ms->meteors[i].object.position = (Vector2){
			.x = (float)GetRandomValue(0, (int)screenDimensions.x),
			.y = (float)GetRandomValue(0, (int)screenDimensions.y),
		};
	}
}

static bool meteor_is_offscreen(struct Asteroid *m, Vector2 screen) {
	float pad = m->radius + 10.0f;
	return m->object.position.x < -pad || m->object.position.y > screen.y + pad;
}

void meteorShower_update(struct MeteorShower *ms) {
	for (int i = 0; i < METEOR_SHOWER_COUNT; i++) {
		struct Asteroid *m = &ms->meteors[i];
		object_move(&m->object);
		if (meteor_is_offscreen(m, ms->screenDimensions)) {
			meteor_init_one(m, ms->screenDimensions);
		}
	}
}

void meteorShower_draw(struct MeteorShower *ms) {
	for (int i = 0; i < METEOR_SHOWER_COUNT; i++) {
		asteroid_draw(&ms->meteors[i]);
	}
}
