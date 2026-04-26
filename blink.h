#pragma once

struct Blink {
	float halfPeriod; // the time of visible / hidden state in seconds
	float _current;   // private
};

// returns true if the component should render itself
bool blink_should_draw(const struct Blink *blink);

// expects the number of seconds since the last call to update
void blink_update(struct Blink *blink, float timeDelta);

// resets the blinker to the initial state
void blink_reset(struct Blink *blink);
