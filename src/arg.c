#include <jlib/arg.h>

bool jlib_arg_check(int argc, char **argv, const char *arg)
{
    for (int i = 0; i < argc; i++) {
        if (streq(argv[i], arg)) {
            return true;
        }
    }
    return false;
}

char *jlib_arg_get(int argc, char **argv, const char *arg)
{
    for (int i = 0; i < argc; i++) {
        if (streq(argv[i], arg) && (i + 1 < argc)) {
            return argv[i + 1];
        }
    }
    return NULL;
}
