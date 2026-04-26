#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>

void *xmalloc(size_t size) {
	void *ptr = malloc(size);
	if (!ptr) {
		fprintf(stderr, "out of memory\n");
		abort();
	}
	return ptr;
}
