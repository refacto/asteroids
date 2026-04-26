#include "blink.h"

void blink_update(struct Blink *blink, float timeDelta) {
	blink->_current += timeDelta;
	float period = 2 * blink->halfPeriod;
	// modulo period
	while (blink->_current > period) {
		blink->_current -= period;
	}
}

void blink_reset(struct Blink *blink) {
	blink->_current = 0;
}

bool blink_should_draw(const struct Blink *blink) {
	return blink->_current <= blink->halfPeriod;
}
