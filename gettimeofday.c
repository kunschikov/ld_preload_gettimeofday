#define _GNU_SOURCE
#include <dlfcn.h>
#include <sys/time.h>
#include <syslog.h>
#include <stdlib.h>
#include <time.h>

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
     static int (*f)(struct timeval *tv, struct timezone *tz) = NULL;
     static int boottime = 0;
     struct timespec ts;
     int clock_gettime_status = clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
     if(!f){
          f = dlsym(RTLD_NEXT, "gettimeofday");
          boottime = time(0) - ts.tv_sec;
     }

     if(!f){
          syslog(LOG_ERR, "failed to find gettimeofday() in current set of shared libraries: %s", dlerror());
          exit(-1);
     }
     int gettimeof_day_status = f(tv, tz);
     if(!clock_gettime_status)
          tv->tv_sec = ts.tv_sec + boottime;
     return gettimeof_day_status;
}
