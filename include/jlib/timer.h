#ifndef JLIB_TIMER_H
#define JLIB_TIMER_H

#include <time.h>

extern clock_t _TimerStart;

/* reset the timer and get current clock */
void timer_rst();

/* get the interval of time in seconds since last reset */
double timer_lap();

#endif /* JLIB_TIMER_H */
