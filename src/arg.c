#include <string.h>

#include <jlib/arg.h>

#define DELVE_DIDNT_FIND_IT -1

static int delve(int argc, char **argv, const char *da, const char *ddarg);

int arg_check(int argc, char **argv, const char *da, const char *ddarg)
{
	int found;
	found = delve(argc, argv, da, ddarg);
	if (found == DELVE_DIDNT_FIND_IT) {
		return 0;
	}
	return found;
}

char *arg_get(int argc, char **argv, const char *da, const char *ddarg)
{
	int found;
	found = delve(argc, argv, da, ddarg);
	if ( (found == DELVE_DIDNT_FIND_IT) || ( (found + 1) >= argc) ) {
		return NULL;
	}
	return argv[found + 1];
}

static int delve(int argc, char **argv, const char *da, const char *ddarg)
{
	int i, found;
	const char *current;

	if (!da && !ddarg)
	{
		goto miss;
	}

	for (i = 0; i < argc; i++) {
		current = argv[i];
		found =
			(da && (strcmp(current, da) == 0) ) ||
			(ddarg && (strcmp(current, ddarg) == 0) );

		/* look for -alghfibcd like things */
		if (!found &&
		    da &&
		    (current[0] == '-') &&
		    (current[1] != '-') &&
		    (current[1] != '\0') )
		{
			found = (strchr(current, da[1]) != NULL);
		}

		if (found) {
			return i;
		}
	}

miss:
	return DELVE_DIDNT_FIND_IT;
}
