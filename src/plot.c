#include <stdarg.h>
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

struct plot *plot_new(unsigned w, unsigned h)
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
	splt->axis_x = calloc(PLOT_DEFAULT_AXIS_COUNT, sizeof(void *));
	if (!splt->axis_x) {
		fputs("Error: Could not calloc for subplot x-axis", stderr);
		exit(-1);
	}
	splt->axis_y = calloc(PLOT_DEFAULT_AXIS_COUNT, sizeof(void *));
	if (!splt->axis_y) {
		fputs("Error: Could not calloc for subplot y-axis", stderr);
		exit(-1);
	}
	
	splt->x = x;
	splt->y = y;
	splt->cap = PLOT_DEFAULT_AXIS_COUNT;
	splt->size = 0;
	splt->title = title;
	splt->count = 0;
	splt->type = PLOT_INVALID;

	size_t index = y * self->w + x;
	self->subplots[index] = splt;
}

void plot_sub_insert(struct plot *self, size_t x, size_t y,
	void *axis_x, void *axis_y, char *options, unsigned type, size_t count)
{
	size_t index = y * self->w + x;
	struct subplot *splt = self->subplots[index];
	if (!splt) {
		fputs("Error: Tried to insert into non-existant subplot", stderr);
		exit(-1);
	}

	/* ensure enough room for plot */
	if (splt->size + 1 == splt->cap) {
		void *tmp;
		splt->cap *= PLOT_DEFAULT_SCALING;

		tmp = realloc(splt->axis_x, splt->cap);
		if (!tmp) {
			fputs("Error: Could not realloc axis_x for subplot insertion", stderr);
			exit(-1);
		}
		splt->axis_x = (void **)tmp;

		tmp = realloc(splt->axis_y, splt->cap);
		if (!tmp) {
			fputs("Error: Could not realloc axis_y for subplot insertion", stderr);
			exit(-1);
		}
		splt->axis_y = (void **)tmp;

		tmp = realloc(splt->options, splt->cap);
		if (!tmp) {
			fputs("Error: Could not realloc options for subplot insertion", stderr);
			exit(-1);
		}
		splt->options = (char **)tmp;
		
		tmp = realloc(splt->count, splt->cap);
		if (!tmp) {
			fputs("Error: Could not realloc count for subplot insertion", stderr);
			exit(-1);
		}
		splt->count = (char **)tmp;
		
		tmp = realloc(splt->type, splt->cap);
		if (!tmp) {
			fputs("Error: Could not realloc type for subplot insertion", stderr);
			exit(-1);
		}
		splt->type = (char **)tmp;
	}

	splt->axis_x[splt->size] = axis_x;
	splt->axis_y[splt->size] = axis_y;
	splt->options[splt->size] = options;
	splt->count[splt->size] = count;
	splt->type[splt->size] = type;
	splt->size++;
}

static void safecat(char *destination, char *source)
{
	void *tmp = realloc(destination, strlen(destination) + strlen(source) + 1);
	if (!tmp) {
		fputs("Error: Could not realloc in safecat", stderr);
		exit(-1);
	}
	strcat(destination, source);
}

static void strcatf(char *dest, const char *fmt, ...)
{
	size_t len = strlen(dest);
	va_list arglist;

	va_start(arglist, fmt);
	vsnprintf(&dest[len], BUFSIZ - 1 - (size_t)len, fmt, arglist);
	va_end(arglist);
}

static char *plot_resolve_str(struct plot *self, size_t index, size_t subindex)
{
	struct subplot *splt = self->subplots[index];
	switch (splt->type) {
		case PLOT_INT:
			return plot_itoa(splt->axis_x, splt->count[subindex]);
		case PLOT_FLOAT:
			return plot_ftoa(splt->axis_x, splt->count[subindex]);
		case PLOT_DOUBLE:
			return plot_dtoa(splt->axis_x, splt->count[subindex]);
		default:
			fputs("Error: Invalid type for plot resolving to string", stderr);
			exit(-1);
	}
}

void plot_show(struct plot *self)
{
	safecat(self->program,
		"from matplotlib import pyplot as plt\n"
	);

	if (self->w == 1 && self->h == 1) {
		safecat(self->program,
			"fig, axes = plt.subplots()\n"
		);
	} else {
		/* TODO: Must safecat plt.subplots( w, h ) */
	}

	/* for each subplot */
	size_t i, j;
	for (i = 0; i < self->w * self->h; i++) {
		char *axis_x, *axis_y, *options;

		/* for each subindex */
		for (j = 0; j < self->subplots[i]->size; j++) {
			axis_x  = plot_resolve_str(self->subplots[i]->axis_x, j);
			axis_y  = plot_resolve_str(self->subplots[i]->axis_y, j);
			options = plot_resolve_str(self->subplots[i]->options, j);
			safecat(self->program, "")
		}
	}

	/*PyRun_SimpleString(
		"x = np.linspace(0, 2*np.pi, 400)\n"
		"y = np.sin(x**2)\n"
		"fig, axes = plt.subplots()\n"
		"axes.plot(x, y)\n"
		"axes.set_title('Simple plot')\n"
	);*/

Show:
	Py_Initialize();

	PyRun_SimpleString(
		"plt.show()\n"
	);

	Py_Finalize();
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
