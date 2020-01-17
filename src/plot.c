#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jlib/plot.h>

#if 0
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

void plot_init()
{
	Py_Initialize();
}

void plot_exit()
{
	Py_Finalize();
}
#endif

struct plot *plot_new(unsigned w, unsigned h, int type)
{
	if (w == 0 || h == 0) {
		fputs("Error: Plot width or height is zero", stderr);
		exit(-1);
	}

	struct plot *self = malloc(sizeof(struct plot));
	if (!self) {
		fputs("Error: Could not malloc for plot init", stderr);
		exit(-1);
	}

	self->program = calloc(PLOT_DEFAULT_PROG_SIZE, sizeof(char));
	if (!self->program) {
		fputs("Error: Could not calloc for plot program", stderr);
		exit(-1);
	}

	self->subplots = malloc(sizeof(struct subplot) * w * h);
	if (!self->subplots) {
		fputs("Error: Could not malloc for plot's subplots", stderr);
		exit(-1);
	}

	self->w = w;
	self->h = h;
	self->type = type;

	return self;
}

void plot_free(struct plot *self)
{
	if (!self)
		return;

	if (self->program)
		free(self->program);
	
	if (self->subplots) {
		size_t i;
		for (i = 0; i < self->w * self->h; i++) {
			if (self->subplots)
				free(self->subplots[i]);
		}
		free(self->subplots);
	}

	free(self);
}

void plot_sub_new(struct plot *self, size_t x, size_t y, char *title)
{
	struct subplot *splt = malloc(sizeof(struct subplot));
	if (!splt) {
		fputs("Error: Could not malloc for subplot", stderr);
		exit(-1);
	}
	splt->args = calloc(PLOT_DEFAULT_ARG_SIZE, sizeof(char));
	splt->title = title;
	splt->argc = 0;

	size_t index = y * self->w + x;

}


char *plot_atoa(void *arr, size_t count,
	void (* _toa)(char *target, void *data, size_t index))
{
	/*                 '[' ']' 'PLOT_MAX_NUMBER_LENGTH'  ','       '\0' */
	char *list = calloc(1 + 1 + (PLOT_MAX_NUMBER_LENGTH + 1)*count + 1, sizeof(char));
	char val[PLOT_MAX_NUMBER_LENGTH] = { 0 };

	strncat(list, "[", 1);

	size_t i, j;
	for (i = 0; i < count; i++) {
		_toa(val, arr, i);
		strncat(list, val, PLOT_MAX_NUMBER_LENGTH);
		strncat(list, ",", 1);
		for (j = 0; j < PLOT_MAX_NUMBER_LENGTH; j++)
			val[j] = 0;
	}

	strncat(list, "]", 1);

	return list;
}

void _plot_itoa(char *target, void *data, size_t index)
{
	itoa(((int *)data)[index], target, 10);
}

void _plot_ftoa(char *target, void *data, size_t index)
{
	snprintf(target, PLOT_MAX_NUMBER_LENGTH, "%f", ((float *)data)[index]);
}

void _plot_dtoa(char *target, void *data, size_t index)
{
	snprintf(target, PLOT_MAX_NUMBER_LENGTH, "%lf", ((double *)data)[index]);
}
