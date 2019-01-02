#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "output.h"

int VERBOSITY = DEFAULT_VERBOSITY;

char *ERROR_CODES[] = {
    "Not defined, see error message (if any).",
    "File not found.",
    "Access violation.",
    "Disk full or allocation exceeded.",
    "Illegal TFTP operation.",
    "Unknown transfer ID.",
    "File already exists.",
    "No such user.",
    "Options refused."};

void print_if_verbose(char *format, ...)
{
  if (VERBOSITY)
  {
    va_list printargs;
    va_start(printargs, format);
    vprintf(format, printargs);
    va_end(printargs);
  }
}

void reporter(int error_number)
{
  if (REPORTING)
  {
    printf("{{err:%d}}\n", error_number);
    fprintf(stderr, "{{err:%d}}\n", error_number);
  }
}

void print_statistics(struct timeval *result, long int total_bytes, int transfer_errors)
{
  long int millis = (result->tv_sec * 1000) + (result->tv_usec / 1000);
  double kBPs = (double) (((float)total_bytes / (float)1000) / ((float)millis / (float)1000));

  printf("{{total_bytes:%lu}}\n", total_bytes);
  printf("{{kbps:%f}}\n", kBPs);
  printf("{{total_time:%lu}}\n", millis);
  printf("{{errors:%d}}\n", transfer_errors);
}
