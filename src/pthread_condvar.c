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

#include <pthread.h>
#include "stub_funcs.h"

struct thread_sync_impl {
	pthread_cond_t cond;
	pthread_mutex_t cond_mutex;
};

static int
sync_impl_init(struct thread_sync_impl *impl)
{
	pthread_cond_init(&impl->cond, NULL);
	pthread_mutex_init(&impl->cond_mutex, NULL);

	return 0;
}

static int
sync_impl_wait(struct thread_sync_impl *impl)
{
	pthread_mutex_lock(&impl->cond_mutex);
	pthread_cond_wait(&impl->cond, &impl->cond_mutex);
	pthread_mutex_unlock(&impl->cond_mutex);

	return 0;
}

static int
sync_impl_signal(struct thread_sync_impl *impl)
{
	pthread_cond_signal(&impl->cond);

	return 0;
}

#include "stub.inc.c"
