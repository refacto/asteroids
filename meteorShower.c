#include "meteorShower.h"
#include <raylib.h>
#include <raymath.h>

// direction: toward bottom-left in screen coords (Y-down)
constexpr int METEOR_DIR_CENTER = 135;
// spread: +-15
constexpr int METEOR_DIR_SPREAD = 15;

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

static bool collides_with_any(struct MeteorShower *ms, int skip, Vector2 pos,
							  float radius) {
	for (int j = 0; j < METEOR_SHOWER_COUNT; j++) {
		if (j == skip) {
			continue;
		}
		float min_dist = radius + ms->meteors[j].radius;
		if (Vector2Distance(pos, ms->meteors[j].object.position) < min_dist) {
			return true;
		}
	}
	return false;
}

static void meteor_spawn(struct Asteroid *m, struct MeteorShower *ms, int idx,
						 Vector2 pos) {
	asteroid_init(m);
	m->object.velocity = ms->velocity;
	for (int attempt = 0; attempt < 50; attempt++) {
		if (!collides_with_any(ms, idx, pos, m->radius)) {
			break;
		}
		pos = meteor_spawn_position(ms->screenDimensions);
	}
	m->object.position = pos;
}

void meteorShower_init(struct MeteorShower *ms, Vector2 screenDimensions) {
	ms->screenDimensions = screenDimensions;
	int deg = GetRandomValue(METEOR_DIR_CENTER - METEOR_DIR_SPREAD,
							 METEOR_DIR_CENTER + METEOR_DIR_SPREAD);
	float speed = (float)GetRandomValue(1, 3);
	ms->velocity = (Vector2){
		.x = speed * cosf((float)deg * DEG2RAD),
		.y = speed * sinf((float)deg * DEG2RAD),
	};
	for (int i = 0; i < METEOR_SHOWER_COUNT; i++) {
		Vector2 pos = {
			.x = (float)GetRandomValue(0, (int)screenDimensions.x),
			.y = (float)GetRandomValue(0, (int)screenDimensions.y),
		};
		meteor_spawn(&ms->meteors[i], ms, i, pos);
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
			meteor_spawn(m, ms, METEOR_SHOWER_COUNT,
						 meteor_spawn_position(ms->screenDimensions));
		}
	}
}

void meteorShower_draw(struct MeteorShower *ms) {
	for (int i = 0; i < METEOR_SHOWER_COUNT; i++) {
		asteroid_draw(&ms->meteors[i]);
	}
}
