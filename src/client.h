#pragma once
#include "options.h"

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

// TFTP opcodes are two bytes fields
#define RRQ_OPCODE_FIELD "\0\x01" // Opcode 1 for Read Request
#define WRQ_OPCODE_FIELD "\0\x02" // Opcode 2 for Write Request
#define DTA_OPCODE_FIELD "\0\x03" // Opcode 3 for Data packet
#define ACK_OPCODE_FIELD "\0\x04" // Opcode 4 for Acknowlege packet
#define ERR_OPCODE_FIELD "\0\x05" // Opcode 5 for Error packet
#define OAC_OPCODE_FIELD "\0\x06" // Opcode 6 for OACK packet

// TFTP opcodes
#define RRQ_OPCODE 1
#define WRQ_OPCODE 2
#define DTA_OPCODE 3
#define ACK_OPCODE 4
#define ERR_OPCODE 5
#define OAC_OPCODE 6

// Default settings
#define MODE "NETASCII"
#define INIT_PORT "69"
#define DEFAULT_BLOCKSIZE 512
#define MAX_RETRIES 4
#define DEFAULT_TIMEOUT_SECS 1
#define DEFAULT_VERBOSITY 0
#define REPORTING 1

char *ERROR_CODES[9];

// Field sizes
#define OPCODE_SIZE 2
#define ACK_SIZE 4

int get(char* target, char* port, char* filename, int timeout_secs, struct OPTION options[], int option_count);
void print_if_verbose(char *format, ...);
void reporter(int error_number);