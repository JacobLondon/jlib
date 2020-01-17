#include <stdio.h>

#include <jlib/plot.h>

void plot_init()
{
	Py_Initialize();
}

void plot_exit()
{
	Py_Finalize();
}

void plot_test()
{
	Py_Initialize();
	PyRun_SimpleString("from time import time,ctime\n"
						"print('Today is',ctime(time()))\n");
	Py_Finalize();
}

void ploti(int *x, const char *xfmt, int *y, const char *yfmt)
{

}

void plotf(double *x, const char *xfmt, double *y, const char *yfmt)
{

}
