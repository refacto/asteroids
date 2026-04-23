#include "screenDimensions.h"
#include <stdlib.h>

static int g_screenWidth = 0;
static int g_screenHeight = 0;

void screenDimensions_set(int width, int height)
{
    if (width <= 0 || height <= 0) 
    {
        abort();
    }
    g_screenWidth = width;
    g_screenHeight = height;
}

int screenWidth_get(void)
{
    return g_screenWidth;
}

int screenHeight_get(void)
{
    return g_screenHeight;
}

Vector2 screenDimensions_get(void)
{
    return (Vector2)
    {
        .x = (float)g_screenWidth,
        .y = (float)g_screenHeight,
    };
}