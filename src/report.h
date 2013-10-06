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

#include <stdio.h>
#include <unistd.h>

/* just a convenience function so that all the different tests output
 * in the same format. */
static inline void
report(useconds_t slept_for, time_t diff_seconds, long diff_nsec)
{
	printf(" :: latency for %d usec: %ld s, %ld ns\n",
			slept_for, diff_seconds, diff_nsec);
}
