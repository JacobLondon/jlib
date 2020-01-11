#include <jlib/timer.h>

clock_t _TimerStart = 0;

void timer_rst()
{
	_TimerStart = clock();
}

double timer_lap()
{
	return ((double)(clock() - _TimerStart)) / (double)CLOCKS_PER_SEC;
}


