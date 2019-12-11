#ifndef JLIB_DEBUG_H
#define JLIB_DEBUG_H

extern int HERE_PASS;
#define HERE_OFF HERE_PASS = 0
#define HERE_ON HERE_PASS = 1

#define HERE(num) \
    if (HERE_PASS) \
        fprintf(stderr, "%s:%s:%d: Here %ld\n", __FILE__, __func__, __LINE__, num)

void halt(char *message);

#endif // JLIB_DEBUG_H
