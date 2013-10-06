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
#include <time.h>

#include "timeutil.h"
#include "report.h"
#include "test_params.h"

int
main(int argc, char **argv)
{
	struct timespec send_time, recv_time, diff;
	struct test_run *run;
	useconds_t sleep_for;
	int times, i = 0;

	for (run = test_runs; run->sleep_for; run++) {
		sleep_for = run->sleep_for;
		times = run->times;

		for (i = 0; i < times; i++) {
			usleep(sleep_for);

			clock_gettime(CLOCK_MONOTONIC, &send_time);
			clock_gettime(CLOCK_MONOTONIC, &recv_time);
			timespec_diff(&diff, &recv_time, &send_time);

			report(sleep_for, diff.tv_sec, diff.tv_nsec);
		}
	}

	return EXIT_SUCCESS;
}
