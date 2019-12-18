#include "arg.h"

bool arg_check(int argc, char **argv, const char *arg)
{
    for (int i = 0; i < argc; i++) {
        if (streq(argv[i], arg)) {
            return true;
        }
    }
    return false;
}

char *arg_get(int argc, char **argv, const char *arg)
{
    for (int i = 0; i < argc; i++) {
        if (streq(argv[i], arg) && (i + 1 < argc)) {
            return argv[i + 1];
        }
    }
    return NULL;
}
