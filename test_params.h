#include <unistd.h>

struct test_run {
	useconds_t sleep_for;
	int times;
};

struct test_run test_runs[] = {
	{1000000, 3},
	{100000, 30},
	{10000, 300},
	{1000, 3000},
	{0}
};
