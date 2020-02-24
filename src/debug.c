#include <stdlib.h>

#include <jlib/debug.h>

void halt(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	exit(1);
}
