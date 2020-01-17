#ifndef JLIB_PLOT_H
#define JLIB_PLOT_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stddef.h> /* size_t */

#define PLOT_MAX_NUMBER_LENGTH 20
#define PLOT_DEFAULT_PROG_SIZE 256
#define PLOT_DEFAULT_ARG_SIZE 128

enum plot_types {
	PLOT_SIMPLE,
	PLOT_SUBPLOT,
	PLOT_INT,
	PLOT_FLOAT,
	PLOT_DOUBLE
};

struct subplot {
	size_t x, y;
	char *args;  /* x-axis, y-axis, options, ... repeat */
	char *title;
	size_t argc;
};

struct plot {
	char *program;
	struct subplot **subplots;
	unsigned w, h;
	unsigned type;
};

struct plot *plot_new(unsigned w, unsigned h, int type);
void plot_free(struct plot *self);
void plot_sub_new(struct plot *self, size_t x, size_t y, char *title);
void plot_sub_insert(struct plot *self, size_t x, size_t y,
	void *axis_x, void *axis_y, char *options, unsigned type, size_t count);
void plot_show(struct plot *self);

#if 0
void _plot_test();

void plot_init();
void plot_exit();
#endif

/* format an array to a Python list, [X,X,X,...]
   pass a function which puts a single item into a string,
   where the string's length is PLOT_MAX_NUMBER_LENGTH or less */
char *plot_atoa(void *arr, size_t count,
	void (* _toa)(char *target, void *data, size_t index));
void _plot_itoa(char *target, void *data, size_t index);
void _plot_ftoa(char *target, void *data, size_t index);
void _plot_dtoa(char *target, void *data, size_t index);

#define plot_itoa(Arr, Count) plot_atoa(Arr, Count, _plot_itoa)
#define plot_ftoa(Arr, Count) plot_atoa(Arr, Count, _plot_ftoa)
#define plot_dtoa(Arr, Count) plot_atoa(Arr, Count, _plot_dtoa)


#endif /* JLIB_PLOT_H */