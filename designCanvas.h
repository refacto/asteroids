#include "screenController.h"
#include <raylib.h>

struct DesignCanvas {};

void designCanvas_init(struct DesignCanvas *canvas);

void designCanvas_update(struct DesignCanvas *canvas);
void designCanvas_destroy(struct DesignCanvas *canvas);

void designCanvas_draw(struct DesignCanvas *canvas);

void designCanvas_screen_draw(void *data);

void designCanvas_screen_update(struct ScreenController *ctrl, void *data);
