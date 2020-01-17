#ifndef JLIB_PLOT_H
#define JLIB_PLOT_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stddef.h> /* size_t */

extern char *_InternalPlotProgram;
#define PLOT_MAX_NUMBER_LENGTH 20

#define plot_init Py_Initialize();
#define plot_exit Py_Finalize();

void _plot_test();

void plot_simple();
char *arri_to_s(int *arr);
char *arrf_to_s(double *arr);

/* format an array to a Python list, [X,X,X,...] 
   pass a function which puts a single item into a string,
   where the string's length is PLOT_MAX_NUMBER_LENGTH or less */
char *plot_fmt_array(void *arr, size_t count,
	void (* stringify)(char *target, void *data, size_t index));
void plot_stringify_int(char *target, void *data, size_t index);
void plot_stringify_float(char *target, void *data, size_t index);
void plot_stringify_double(char *target, void *data, size_t index);

#define plot_fmt_i(Arr, Count) plot_fmt_array(Arr, Count, plot_stringify_int)
#define plot_fmt_f(Arr, Count) plot_fmt_array(Arr, Count, plot_stringify_float)
#define plot_fmt_lf(Arr, Count) plot_fmt_array(Arr, Count, plot_stringify_double)


#endif /* JLIB_PLOT_H */