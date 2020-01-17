#ifndef JLIB_PLOT_H
#define JLIB_PLOT_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

void plot_init();
void plot_exit();

void plot_test();

void ploti(int *x, const char *xfmt, int *y, const char *yfmt);
void plotf(double *x, const char *xfmt, double *y, const char *yfmt);

#endif /* JLIB_PLOT_H */