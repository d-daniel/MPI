#include "walltime.h"

double walltime(void) {
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  return(tv.tv_sec + tv.tv_usec/1000000.0);
}
