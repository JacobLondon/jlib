#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

void halt(char *message)
{
    fprintf(stderr, "Error: %s\n", message);
    exit(-1);
}