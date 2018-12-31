#pragma once
#include "client.h"
#include <stdlib.h>
#include <sys/time.h>

#define DEFAULT_VERBOSITY 0
#define REPORTING 1

char *ERROR_CODES[9];
int VERBOSITY;

#define USAGE "\
Usage: ntftp [-options] target filename\n\
  Available options:\n\
    -p PORT: Port used to make initial request on\n\
    -b BLOCKSIZE: Set the blocksize, using option protocol\n\
       unless silenced by -B\n\
    -B: Silence blocksize option. If this option is present,\n\
       do not use option protocol to declare it to the\n\
       server, simply behave in the specified manner. Useful\n\
       when a server does not support the option extension\n\
       but is preconfigured with a certain option.\n\
    -w WINDOWSIZE: Set the windowsize, using option protocol\n\
       unless silenced by -W\n\
    -W: Silence windowsize option.\n\
    -t TIMEOUT: Set receive timeout (seconds). Default 1s.\n\
    -V: Verbose mode.\n\
    -h/-H: display this message\n\
\nMaintainer: Isaiah Taylor <Isaiah.Taylor@netscout.com>\n"

void print_if_verbose(char *format, ...);

void reporter(int error_number);

void print_statistics(struct timeval *result, long int total_bytes);
