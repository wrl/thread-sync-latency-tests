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
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "timeutil.h"
#include "report.h"
#include "test_params.h"

#include "stub_funcs.h"

struct thread_sync {
	struct thread_sync_impl impl;
	struct timespec send_time;
	useconds_t sleep_for;
};

struct thread_pair {
	pthread_t reader;
	pthread_t writer;

	struct thread_sync tsync;
};

static void *
writer_thread(void *ctx)
{
	struct thread_sync_impl *impl;
	struct thread_sync *tsync;
	struct timespec *send_time;
	useconds_t sleep_for;

	tsync = ctx;
	send_time = &tsync->send_time;
	sleep_for = tsync->sleep_for;
	impl = &tsync->impl;

	for (;;) {
		usleep(sleep_for);
		clock_gettime(CLOCK_MONOTONIC, send_time);
		sync_impl_signal(impl);
	}

	return NULL;
}

static void *
reader_thread(void *ctx)
{
	struct timespec *send_time, recv_time, diff;
	struct thread_sync_impl *impl;
	struct thread_sync *tsync;
	useconds_t sleep_for;

	tsync = ctx;
	send_time = &tsync->send_time;
	sleep_for = tsync->sleep_for;
	impl = &tsync->impl;

	for (;;) {
		sync_impl_wait(impl);

		clock_gettime(CLOCK_MONOTONIC, &recv_time);
		timespec_diff(&diff, &recv_time, send_time);
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

static int
pair_init(struct thread_pair *pair, useconds_t sleep_time)
{
	pair->tsync.sleep_for = sleep_time;
	sync_impl_init(&pair->tsync.impl);

	pthread_create(&pair->reader, NULL, reader_thread, &pair->tsync);
	pthread_create(&pair->writer, NULL, writer_thread, &pair->tsync);

	return 0;
}

int
main(int argc, char **argv)
{
	struct thread_pair *pairs;
	useconds_t terminate_in;
	pthread_t timer;
	int i;

	terminate_in = TEST_RUN_FOR_SECONDS * 1000000;

	for (i = 0; TEST_RUNS[i]; i++);
	pairs = calloc(1, sizeof(*pairs));

	for (i = 0; TEST_RUNS[i]; i++)
		pair_init(&pairs[i], TEST_RUNS[i]);

	pthread_create(&timer, NULL, timer_thread, &terminate_in);
	pthread_join(timer, NULL);

	return EXIT_SUCCESS;
}
