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

// Field sizes
#define OPCODE_SIZE 2
#define ACK_SIZE 4

// Error codes
char *ERROR_CODES[] = {
  "Not defined, see error message (if any).",
  "File not found.",
  "Access violation.",
  "Disk full or allocation exceeded.",
  "Illegal TFTP operation.",
  "Unknown transfer ID.",
  "File already exists.",
  "No such user."
  "Options refused."
};

#define MODE "NETASCII"
#define INIT_PORT "69"
#define BUFFER_LENGTH 550

// Options extension, RFC 2347

#define WINDOWSIZE_FLAG "windowsize"
#define BLOCKSIZE_FLAG "blocksize"
#define OACK_MAX_SIZE 512

struct OPTION {char *name; char *value; int silent;};

int get(char* target, char* port, char* filename, struct OPTION options[], int option_count);
