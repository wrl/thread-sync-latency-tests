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

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "stub_funcs.h"

struct thread_sync_impl {
	int fds[2];
};

static int
sync_impl_init(struct thread_sync_impl *impl)
{
	int i, fd, flags;

	pipe(impl->fds);

	for (i = 0; i < 2; i++) {
		fd = impl->fds[i];
		flags = fcntl(fd, F_GETFL, 0);
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	}

	return 0;
}

static int
sync_impl_wait(struct thread_sync_impl *impl)
{
	struct pollfd fds[1];
	uint8_t buf;

	fds[0].fd = impl->fds[0];
	fds[0].events = POLLIN;

	poll(fds, 1, -1);
	read(impl->fds[0], &buf, 1);
	return 0;
}

static int
sync_impl_signal(struct thread_sync_impl *impl)
{
	uint8_t buf = 0;
	write(impl->fds[1], &buf, 1);
	return 0;
}

#include "stub.inc.c"
