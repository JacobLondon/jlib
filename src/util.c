#include <stdlib.h>

#include <jlib/util.h>

void xswap(void *x, void *y)
{
	x = (void *)((size_t)x ^ (size_t)y);
	y = (void *)((size_t)y ^ (size_t)x);
	x = (void *)((size_t)x ^ (size_t)y);
}

void swap(void *x, void *y)
{
	void *a = x;
	x = y;
	y = a;
}

void *copy(void *buf, size_t size)
{
	char *new = calloc(size, sizeof(char));
	if (!new) {
		return NULL;
	}
	size_t i;
	for (i = 0; i < size; i++) {
		new[i] = ((char *)buf)[i];
	}
	return (void *)new;
}
