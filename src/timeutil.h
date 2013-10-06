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

/* copy with nanosecond increment.
 * handles wrap-around properly. */
static inline void
timespec_copy_inc(struct timespec *dst, struct timespec *src, long by_nsec)
{
	dst->tv_sec  = src->tv_sec;
	dst->tv_nsec = src->tv_nsec + by_nsec;

	if (dst->tv_nsec > 999999999) {
		dst->tv_sec++;
		dst->tv_nsec -= 999999999;
	} else if (dst->tv_nsec < 0) {
		dst->tv_sec--;
		dst->tv_nsec += 999999999;
	}
}


static inline void
timespec_diff(struct timespec *diff, struct timespec *a, struct timespec *b)
{
	diff->tv_sec  = a->tv_sec  - b->tv_sec;
	diff->tv_nsec = a->tv_nsec - b->tv_nsec;
}

/* timespec comparison.
 * granularity specified in fraction of a second. i.e. pass `1`
 * for a granularity of 1 second, `1000` for a millisecond, etc.
 *
 * finest granularity is 1 nanosecond, coarsest is 1 second. */
static inline int
timespec_cmp(struct timespec *a, struct timespec *b, long granularity)
{
	struct timespec diff;
	int64_t nsec_diff;

	timespec_diff(&diff, a, b);

	if (granularity < 1 || granularity > 1000000000L)
		granularity = 1;
	else
		granularity = 1000000000L / granularity;

	nsec_diff = (diff.tv_sec * 1000000000L) +
		((diff.tv_nsec / (granularity)) * granularity);

	if (nsec_diff > 0)
		return 1;
	else if (nsec_diff < 0)
		return -1;

	return 0;
}
