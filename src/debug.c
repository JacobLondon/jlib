#include <stdlib.h>
#include <jlib/debug.h>

int jlib_HerePass = 1;

void jlib_halt(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	exit(-1);
}