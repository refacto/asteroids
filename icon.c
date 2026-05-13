#include "icon.h"
#include <raylib.h>
#include <stddef.h>
#include <stdlib.h>

constexpr unsigned char iconData[] = {
#embed "resources/icons/icon.png"
};
static_assert((sizeof iconData) >= 4,
			  "There should be at least 4 elements in this array.");
constexpr size_t iconSize = sizeof(iconData);

Image icon_load() {
	return LoadImageFromMemory(".png", iconData, iconSize);
}
