#pragma once
#include <raylib.h>

// call function once in main.c
void screenDimensions_set(int width, int height);

// read screen dimensions anywhere
int screenWidth_get(void);
int screenHeight_get(void);
Vector2 screenDimensions_get(void);