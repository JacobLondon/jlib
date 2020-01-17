#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jlib/plot.h>

char *_InternalPlotProgram;

void _plot_test()
{
	Py_Initialize();
	/*PyRun_SimpleString("from time import time,ctime\n"
						"print('Today is',ctime(time()))\n");*/
	PyRun_SimpleString(
		"from matplotlib import pyplot as plt\n"
		"import numpy as np\n"
	);
	PyRun_SimpleString(
		"x = np.linspace(0, 2*np.pi, 400)\n"
		"y = np.sin(x**2)\n"
		"fig, ax = plt.subplots()\n"
		"ax.plot(x, y)\n"
		"ax.set_title('Simple plot')\n"
		"plt.show()\n"
	);
	Py_Finalize();
}

char *plot_fmt_array(void *arr, size_t count,
	void (* stringify)(char *target, void *data, size_t index))
{
	/*                 '[' ']' 'PLOT_MAX_NUMBER_LENGTH'  ','       '\0' */
	char *list = calloc(1 + 1 + (PLOT_MAX_NUMBER_LENGTH + 1)*count + 1, sizeof(char));
	char val[PLOT_MAX_NUMBER_LENGTH] = { 0 };

	strncat(list, "[", 1);

	size_t i, j;
	for (i = 0; i < count; i++) {
		stringify(val, arr, i);
		strncat(list, val, PLOT_MAX_NUMBER_LENGTH);
		strncat(list, ",", 1);
		for (j = 0; j < PLOT_MAX_NUMBER_LENGTH; j++)
			val[j] = 0;
	}

	strncat(list, "]", 1);

	return list;
}

void plot_stringify_int(char *target, void *data, size_t index)
{
	snprintf(target, PLOT_MAX_NUMBER_LENGTH, "%d", ((int *)data)[index]);
}

void plot_stringify_float(char *target, void *data, size_t index)
{
	snprintf(target, PLOT_MAX_NUMBER_LENGTH, "%f", ((float *)data)[index]);
}

void plot_stringify_double(char *target, void *data, size_t index)
{
	snprintf(target, PLOT_MAX_NUMBER_LENGTH, "%lf", ((double *)data)[index]);
}
