#include <jlib/timer.h>

clock_t _InternalTimerStar = 0;

void timer_rst()
{
	_InternalTimerStart = clock();
}

double timer_lap()
{
	return ((double)(clock() - _InternalTimerStart)) / (double)CLOCKS_PER_SEC;
}

double timer_step()
{
	double t = timer_lap();
	timer_rst();
	return t;
}
