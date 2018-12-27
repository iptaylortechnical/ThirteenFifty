#define RRQ_OPCODE "\0\x01" // Two byte field with opcode 1
#define ACK_SIZE = 4
#define ACK_OPCODE = "\0\x04" // Two byte field with opcode 4
#define MODE "NETASCII"
#define INIT_PORT "69"
#define BUFFER_LENGTH 550

int get(char* target, char* port, char* filename);
