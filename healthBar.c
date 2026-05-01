#include "healthBar.h"
#include "heart.h"

constexpr float HEART_SCALE = 20.0f;

static void init_hearts(struct HealthBar *bar) {
	// hearts are a bit funny, they center at the square midpoint
	// so we manually pad it here
	Vector2 position = {
		.x = bar->position.x + 0.5f * HEART_SCALE,
		.y = bar->position.y + 0.5f * HEART_SCALE,
	};
	for (int i = 0; i < MAX_LIVES; i++) {
		heart_init(&bar->hearts[i], HEART_SCALE, position, RED);
		position.x += 2 * HEART_SCALE;
	}
}

void healthBar_init(struct HealthBar *bar, Vector2 position, int numLives) {
	if (numLives > MAX_LIVES) {
		numLives = MAX_LIVES;
	}
	*bar = (struct HealthBar){
		.numLives = numLives,
		.position = position,
	};
	init_hearts(bar);
}

void healthBar_set_num_lifes(struct HealthBar *bar, int numLives) {
	bar->numLives = numLives;
}

void healthBar_draw(struct HealthBar *bar) {
	for (int i = 0; i < bar->numLives; i++) {
		heart_draw(&bar->hearts[i]);
	}
}
