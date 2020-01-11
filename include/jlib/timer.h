#ifndef JLIB_TIMER_H
#define JLIB_TIMER_H

#include <time.h>

extern clock_t _InternalTimerStart;

/* reset the timer and get current clock */
void timer_rst();

/* get the interval of time in seconds since last reset */
double timer_lap();

/* get the interval of time in seconds, and reset the timer */
double timer_step();

#endif /* JLIB_TIMER_H */
