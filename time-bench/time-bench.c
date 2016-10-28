// Quick and dirty program to bench some various clocks

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/time.h>

#define RUNS 1000000
#define NANOS_IN_SECOND 1000000000

struct timespec get_time()
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC_PRECISE, &ts) != 0)
    {
        perror("Unable to get time: ");
        exit(1);
    }

    return ts;
}

time_t get_clock_gettime(clockid_t clock_id)
{
    struct timespec ts;
    if (clock_gettime(clock_id, &ts) != 0)
    {
        perror("Unable to get time: ");
        exit(1);
    }

    return ts.tv_sec;
}

time_t get_gettimeofday(clockid_t clock_id)
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) != 0)
    {
        perror("Unable to get time: ");
        exit(1);
    }

    return tv.tv_sec;
}

uint64_t tv_interval(const struct timespec start, const struct timespec end)
{
    return (end.tv_sec - start.tv_sec) * NANOS_IN_SECOND + (end.tv_nsec - start.tv_nsec);
}

void bench(const char* test_name, time_t (*time_function)(clockid_t), clockid_t clock_id)
{
    int i;
    struct timespec start, end;
   
    start = get_time();

    for (i = 0; i < RUNS; i++)
    {
        (*time_function)(clock_id);
    }

    end = get_time();

    time_t elapsed = tv_interval(start, end);
    printf("%s: %ld ns total, %f ns avg\n", test_name, elapsed, (double)elapsed / RUNS);
}


int main()
{
    bench("gettimeofday", &get_gettimeofday, 0);
    bench("clock_gettime(CLOCK_REALTIME)", &get_clock_gettime, CLOCK_REALTIME);
    bench("clock_gettime(CLOCK_REALTIME_FAST)", &get_clock_gettime, CLOCK_REALTIME_FAST);
    bench("clock_gettime(CLOCK_REALTIME_PRECISE)", &get_clock_gettime, CLOCK_REALTIME_PRECISE);
    bench("clock_gettime(CLOCK_MONOTONIC)", &get_clock_gettime, CLOCK_MONOTONIC);
    bench("clock_gettime(CLOCK_MONOTONIC_FAST)", &get_clock_gettime, CLOCK_MONOTONIC_FAST);
    bench("clock_gettime(CLOCK_MONOTONIC_PRECISE)", &get_clock_gettime, CLOCK_MONOTONIC_PRECISE);
    bench("clock_gettime(CLOCK_SECOND)", &get_clock_gettime, CLOCK_SECOND);
}
