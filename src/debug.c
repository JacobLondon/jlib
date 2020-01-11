#include <stdlib.h>

#include <jlib/debug.h>

int _InternalHerePass = 1;

void halt(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	exit(-1);
}
