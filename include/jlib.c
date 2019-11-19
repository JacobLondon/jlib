#include "jlib.h"

JLib jlib;

int jlib_init()
{
    jlib.array.new  = array_new;
    jlib.array.free = array_free;
    jlib.array.push = array_push;
    jlib.array.pop  = array_pop;

    return 1;
}