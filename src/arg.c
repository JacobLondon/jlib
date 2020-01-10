#include <string.h>

#include <jlib/arg.h>

int arg_check(int argc, char **argv, const char *arg)
{
	int i;
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], arg) == 0) {
			return 1;
		}
	}
	return 0;
}

char *arg_get(int argc, char **argv, const char *arg)
{
	int i;
	for (i = 0; i < argc; i++) {
		if ((strcmp(argv[i], arg) == 0) && (i + 1 < argc)) {
			return argv[i + 1];
		}
	}
	return NULL;
}
