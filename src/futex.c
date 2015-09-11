/**
 * thread_sync_latency
 * benchmarks for the latency of various inter-thread synchronization
 * primitives
 *
 * Written in 2015 by Andrew Kelley <superjoe30@gmail.com>
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

#include <linux/futex.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/errno.h>
#include "stub_funcs.h"

struct thread_sync_impl {
    int n;
};

static inline int futex(int *uaddr, int op, int val,
        const struct timespec *timeout, int *uaddr2, int val3)
{
    return syscall(SYS_futex, uaddr, op, val, timeout, uaddr2, val3);
}

static inline int futex_wait(int *address, int val) {
    return futex(address, FUTEX_WAIT, val, NULL, NULL, 0) ? errno : 0;
}

static inline int futex_wake(int *address, int count) {
    return futex(address, FUTEX_WAKE, count, NULL, NULL, 0) ? errno : 0;
}

static int
sync_impl_init(struct thread_sync_impl *impl)
{
    impl->n = 0;
    return 0;
}

static int
sync_impl_wait(struct thread_sync_impl *impl)
{
    futex_wait(&impl->n, impl->n);
    return 0;
}

static int
sync_impl_signal(struct thread_sync_impl *impl)
{
    futex_wake(&impl->n, 1);
    return 0;
}

#include "stub.inc.c"
