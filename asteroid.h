#pragma once
#include "objects.h"

constexpr int ASTEROID_NUM_POINTS = 10;
constexpr int ASTEROID_MAX_NUM_POINTS = 10;

enum AsteroidSize {
	SIZE_SMALL,
	SIZE_MEDIUM,
	SIZE_LARGE,
	_SIZE_MAX, // EOL
};

struct Asteroid {
	struct Object object;
	enum AsteroidSize size;
	// raw shape points, centered at the origin
	Vector2 points[ASTEROID_MAX_NUM_POINTS];
	// logical shape points, rotated and centered around the position
	Vector2 transformedPoints[ASTEROID_MAX_NUM_POINTS];
	// intrusive linked list
	struct Asteroid *next;
};

// mallocs a new asteroid
struct Asteroid *asteroid_new();

void asteroid_init(struct Asteroid *asteroid);

// check if asteroid list has a next element
bool asteroid_has_next(const struct Asteroid *asteroid);

// add new to end of asteroid list, rooted at head
void asteroid_add(struct Asteroid *head, struct Asteroid *new);

// remove the asteroid from the list rooted at head
// this will update the head pointer if needed
void asteroid_remove(struct Asteroid **head, struct Asteroid *target);

// get the next asteroid element from the list
struct Asteroid *asteroid_next(const struct Asteroid *asteroid);

void asteroid_set_next(struct Asteroid *asteroid, struct Asteroid *next);

// draws the asteroid
void asteroid_draw(struct Asteroid *asteroid);

void asteroid_update(struct Asteroid *asteroid);

void asteroid_set_velocity(struct Asteroid *asteroid, Vector2 velocity);

void asteroid_set_position(struct Asteroid *asteroid, Vector2 position);

// returns true if an asteroid can be split into smaller versions
bool asteroid_can_split(const struct Asteroid *asteroid);

// split the asteroid in multiple pieces, returned as a linked list
// damageDirection indicates in what direction the hit occured
struct Asteroid *asteroid_split(const struct Asteroid *asteroid,
								Vector2 damageDirection);

// checks if a midpoint of a circle hits the asteroid
// this combines both coarse and fine collision detection and should
// preferably be used if able
bool asteroid_collide_circle(struct Asteroid *asteroid, Vector2 position,
							 float radius);

// only checks if the outer asteroid circle hits, this is not yet a collision
bool asteroid_collide_circle_coarse(struct Asteroid *asteroid, Vector2 position,
									float radius);

// fine grained collision detection of a point with the asteroid mesh, expensive
bool asteroid_collide_point(struct Asteroid *asteroid, Vector2 point);

// collision between two asteroids
bool asteroid_collide_asteroid(struct Asteroid *self, struct Asteroid *other);

// TODO: remove, just here for demo
void asteroid_stop_moving(struct Asteroid *asteroid);
