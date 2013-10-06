/**
 * thread_sync_latency
 * benchmarks for the latency of various inter-thread synchronization
 * primitives
 *
 * Written in 2013 by William Light <wrl@illest.net>
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to the
 * public domain worldwide. This software is distributed without any
 * warranty. 
 *
 * You should have received a copy of the CC0 Public Domain Dedication
 * along with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>. 
 */

#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "timeutil.h"
#include "report.h"
#include "test_params.h"

static void *
rw_thread(void *ctx)
{
	struct timespec send_time, recv_time, diff;
	useconds_t sleep_for;

	sleep_for = *((useconds_t *) ctx);

	for (;;) {
		usleep(sleep_for);
		clock_gettime(CLOCK_MONOTONIC, &send_time);
		clock_gettime(CLOCK_MONOTONIC, &recv_time);
		timespec_diff(&diff, &recv_time, &send_time);
		report(sleep_for, diff.tv_sec, diff.tv_nsec);
	}

	return NULL;
}

static void *
timer_thread(void *ctx)
{
	useconds_t *terminate_in = ctx;
	usleep(*terminate_in);
	return NULL;
}

int
main(int argc, char **argv)
{
	pthread_t *rwthreads, timer;
	useconds_t terminate_in;
	int i;

	terminate_in = TEST_RUN_FOR_SECONDS * 1000000;

	for (i = 0; TEST_RUNS[i]; i++);
	rwthreads = calloc(1, sizeof(*rwthreads));

	for (i = 0; TEST_RUNS[i]; i++)
		pthread_create(&rwthreads[i], NULL, rw_thread, (void *) &TEST_RUNS[i]);

	pthread_create(&timer, NULL, timer_thread, &terminate_in);
	pthread_join(timer, NULL);

	return EXIT_SUCCESS;
}
