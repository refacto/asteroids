#include "screenDimensions.h"

static int screenWidth = 0;
static int screenHeight = 0;

void screenDimensions_set(int width, int height) {
	screenWidth = width;
	screenHeight = height;
}

int screenWidth_get() {
	return screenWidth;
}

int screenHeight_get() {
	return screenHeight;
}

Vector2 screenDimensions_get() {
	return (Vector2){
		.x = (float)screenWidth,
		.y = (float)screenHeight,
	};
}
