#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <assert.h>

double get_timer_diff(struct timespec *before, struct timespec *after)
{
  double after_sec = (double)after->tv_sec + (double)after->tv_nsec / 1E9;
  double before_sec = (double)before->tv_sec + (double)before->tv_nsec / 1E9;
	return after_sec - before_sec;
}

void start_watch(struct timespec *timer)
{
	if (clock_gettime(CLOCK_MONOTONIC, timer))
		fprintf(stderr, "startWatch failed\n");
}

void stop_watch(struct timespec *timer)
{
	return start_watch(timer);
}

