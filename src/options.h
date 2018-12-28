// Options extension, RFC 2347

#define WINDOWSIZE_FLAG "windowsize"
#define BLOCKSIZE_FLAG "blocksize"
#define OACK_MAX_SIZE 512

char *ERROR_CODES[8];

// Field sizes
#define OPCODE_SIZE 2
#define ACK_SIZE 4

struct OPTION {char *name; char *value; int silent;};

void create_options(char *rrq_ptr, int rrq_raw_len, struct OPTION options[], int option_count);

int find_option_length(struct OPTION options[], int option_count);

void set_option_flags(struct OPTION options[], int option_count, int *blocksize_ptr, int *windowsize_ptr);

int process_oack(char *oack, int *blocksize_ptr, int *windowsize_ptr);

void create_oack_err(char *error, int error_length);


