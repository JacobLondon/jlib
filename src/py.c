#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jlib/py.h>

#define STR_SINGLE_MAX 20

char *python_atoa(char *prefix, void *arr, size_t count,
	void (* _toa)(char *target, void *data, size_t index))
{
	/*                  prefix          '[' ']' 'PLOT_MAX_NUMBER_LENGTH,'   '\n''\0' */
	char *list = calloc(STR_SINGLE_MAX + 1 + 1 + (STR_SINGLE_MAX + 1)*count + 1 + 1, sizeof(char));
	char val[STR_SINGLE_MAX] = { 0 };

	strncat(list, prefix, STR_SINGLE_MAX);
	strncat(list, "[", 1);

	size_t i, j;
	for (i = 0; i < count; i++) {
		_toa(val, arr, i);
		strncat(list, val, STR_SINGLE_MAX);
		strncat(list, ",", 1);
		for (j = 0; j < STR_SINGLE_MAX; j++)
			val[j] = 0;
	}

	strncat(list, "]\n", 2);

	return list;
}

void _python_itoa(char *target, void *data, size_t index)
{
	snprintf(target, STR_SINGLE_MAX, "%d", ((int *)data)[index]);
}

void _python_ftoa(char *target, void *data, size_t index)
{
	snprintf(target, STR_SINGLE_MAX, "%f", ((float *)data)[index]);
}

void _python_dtoa(char *target, void *data, size_t index)
{
	snprintf(target, STR_SINGLE_MAX, "%lf", ((double *)data)[index]);
}
