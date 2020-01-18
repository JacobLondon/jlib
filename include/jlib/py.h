#ifndef JLIB_PLOT_H
#define JLIB_PLOT_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stddef.h> /* size_t */

#define python_init Py_Initialize
#define python_exit Py_Finalize
#define python_run  PyRun_SimpleString

/* format an array to a Python list, [X,X,X,...]
   pass a function which puts a single item into a string,
   where the string's length is PLOT_MAX_NUMBER_LENGTH or less */
char *python_atoa(char *prefix, void *arr, size_t count,
	void (* _toa)(char *target, void *data, size_t index));
void _python_itoa(char *target, void *data, size_t index);
void _python_ftoa(char *target, void *data, size_t index);
void _python_dtoa(char *target, void *data, size_t index);

#define python_iatoa(Varstr, Arr, Count) python_atoa(Varstr "=", Arr, Count, _python_itoa)
#define python_fatoa(Varstr, Arr, Count) python_atoa(Varstr "=", Arr, Count, _python_ftoa)
#define python_datoa(Varstr, Arr, Count) python_atoa(Varstr "=", Arr, Count, _python_dtoa)


#endif /* JLIB_PLOT_H */