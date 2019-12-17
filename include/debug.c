#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

int HERE_PASS = 1;

void halt(char *message)
{
    fprintf(stderr, "Error: %s\n", message);
    exit(-1);
}