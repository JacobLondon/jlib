#include <assert.h>
#include <jlib/func.h>

int any(void *array, size_t size, size_t step, int (*holds_true)(void *value))
{
	size_t i;
	char *_array = (char *)array;

	if (!array || !holds_true) {
		return 0;
	}

	for (i = 0; i < size * step; i += step) {
		if (holds_true((void *)&_array[i])) {
			return 1;
		}
	}

	return 0;
}

int all(void *array, size_t size, size_t step, int (*holds_true)(void *value))
{
	size_t i, c;
	char *_array = (char *)array;

	if (!array || !holds_true) {
		return 0;
	}

	c = 0;
	for (i = 0; i < size * step; i += step) {
		if (holds_true((void *)&_array[i])) {
			c++;
		}
	}

	return c == size;
}
