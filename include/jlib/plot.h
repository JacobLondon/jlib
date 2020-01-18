#ifndef JLIB_PLOT_H
#define JLIB_PLOT_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stddef.h> /* size_t */

#define PLOT_MAX_NUMBER_LENGTH 20
#define PLOT_DEFAULT_PROG_SIZE 256
#define PLOT_DEFAULT_ARG_SIZE 128
#define PLOT_DEFAULT_AXIS_COUNT 1
#define PLOT_DEFAULT_SCALING 2

enum plot_types {
	PLOT_INVALID,
	PLOT_SIMPLE,
	PLOT_SUBPLOT,
	PLOT_INT,
	PLOT_FLOAT,
	PLOT_DOUBLE
};

struct layer {
	void *axis_x, *axis_y;
	char *options;
	size_t count;
	unsigned type;
};

struct subplot {
	size_t x, y;
	size_t cap, size; /* size and capacity of axis_x/y and options */
	/*void **axis_x, **axis_y; /* hold pointers to different sets of data */
	/*char **options; /* options for each data set */
	char *title;
	/*size_t *count;  /* num of elements in each axis */
	/*unsigned *type; /* type of elements in each axis */
	struct layer *layers;
};

struct plot {
	char *program;
	struct subplot **subplots;
	unsigned w, h;
};

struct plot *plot_new(unsigned w, unsigned h);
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