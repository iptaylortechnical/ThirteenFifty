#pragma once
// Options extension, RFC 2347
#define WINDOWSIZE_FLAG "windowsize"
#define BLOCKSIZE_FLAG "blksize"
#define OACK_MAX_SIZE 512

struct OPTION {char *name; char *value; int silent;};

void create_options(char *rrq_ptr, int rrq_raw_len, struct OPTION options[], int option_count);

int find_option_length(struct OPTION options[], int option_count);

void set_option_flags(struct OPTION options[], int option_count, int *blocksize_ptr, int *windowsize_ptr);

int process_oack(char *oack, int *blocksize_ptr, int *windowsize_ptr);

void create_oack_err(char *error, int error_length);


