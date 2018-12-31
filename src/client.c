#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>

#include "client.h"

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

int socket_setup(char *target, char *port, struct addrinfo *hts, struct addrinfo *info, struct addrinfo **temp_sock)
{

  int fd;
  if ((getaddrinfo(target, port, hts, &info)) != 0)
  {
    print_if_verbose("Could not get address info.");
    exit(1);
  }
  for ((*temp_sock) = info; (*temp_sock) != NULL; (*temp_sock) = (*temp_sock)->ai_next)
  {
    if ((fd = socket((*temp_sock)->ai_family, (*temp_sock)->ai_socktype, (*temp_sock)->ai_protocol)) == -1)
      continue;
    break;
  }

  if (*temp_sock == NULL)
  {
    print_if_verbose("Could not create a socket with those DNS addresses.");
    exit(2);
  }

  // Configure socket timeout
  struct timeval timeout;
  timeout.tv_sec = DEFAULT_TIMEOUT_SECS;
  timeout.tv_usec = 0;
  if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0)
  {
    perror("Could not set socket timeout");
  }

  return fd;
}

int create_rrq(char *filename, char *packet, int rrq_packet_size)
{
  memset(packet, 0, rrq_packet_size);

  packet[0] = 0;
  packet[1] = RRQ_OPCODE;

  strncat(&packet[2], filename, strlen(filename));
  strncat(&packet[2 + strlen(filename) + 1], MODE, strlen(MODE));

  return 2 + strlen(filename) + 1 + strlen(MODE) + 1;
}

void create_ack(char *block_num, char *packet, int ack_packet_size)
{
  memset(packet, 0, ack_packet_size);

  packet[0] = 0;
  packet[1] = ACK_OPCODE;

  packet[2] = block_num[0];
  packet[3] = block_num[1];
}

int send_packet(int fd, char *ack_packet, int ack_packet_size, struct sockaddr *incoming_addr, int addr_len, char *descriptor)
{
  int acknumbytes;
  if ((acknumbytes = sendto(fd, ack_packet, ack_packet_size, 0, incoming_addr, addr_len)) == -1)
  {
    char error_string[50];
    sprintf(error_string, "Sending %s", descriptor);
    perror(error_string);
    exit(1);
  }
  return acknumbytes;
}

int get(char *target, char *port, char *filename, struct OPTION options[], int option_count)
{
  // Initializing options
  int BLOCKSIZE_OPTION = DEFAULT_BLOCKSIZE;
  int WINDOWSIZE_OPTION = (int)NULL;
  int error_length = 2 + 2 + strlen(ERROR_CODES[7]) + 1;
  int option_length = 0;
  if (options)
  {
    option_length = find_option_length(options, option_count);
    set_option_flags(options, option_count, &BLOCKSIZE_OPTION, &WINDOWSIZE_OPTION);
  }

  // Initializing sockets
  struct addrinfo hts, *info, *temp_sock;
  int fd;
  memset(&hts, 0, sizeof hts);
  hts.ai_family = AF_UNSPEC;
  hts.ai_socktype = SOCK_DGRAM;
  struct sockaddr_storage incoming_addr;
  socklen_t addr_len = sizeof incoming_addr;
  int buffer_length = (BLOCKSIZE_OPTION ? BLOCKSIZE_OPTION : DEFAULT_BLOCKSIZE) + 38;
  char recv_buffer[buffer_length];
  char block_num[2];
  int numbytes;
  int acknumbytes;

  // Allocating packet buffers
  size_t rrq_packet_size = 2 + strlen(filename) + 1 + strlen(MODE) + 1 + option_length;
  char *rrq_packet = malloc(rrq_packet_size);
  size_t ack_packet_size = ACK_SIZE;
  char *ack_packet = malloc(ack_packet_size);

  // Configuring socket
  fd = socket_setup(target, port, &hts, info, &temp_sock);

  // create RRQ
  int rrq_raw_len = create_rrq(filename, rrq_packet, rrq_packet_size);
  if (options)
    create_options(rrq_packet, rrq_raw_len, options, option_count);

  // send RRQ
  numbytes = send_packet(fd, rrq_packet, rrq_packet_size, temp_sock->ai_addr, temp_sock->ai_addrlen, "RRQ");
  print_if_verbose("Sent %d bytes to %s on port %s\n", numbytes, target, port);

  // loop: listen for DATA, send ACK
  int iter = 0;
  int window_iter = 1;
  int is_oack = 0;
  int retry_count = 0;
  do
  {
    is_oack = 0;

    if ((numbytes = recvfrom(fd, recv_buffer, buffer_length - 1, 0, (struct sockaddr *)&incoming_addr, &addr_len)) == -1)
    { // Handle recvfrom failure
      if (iter == 0)
      {
        print_if_verbose("Did not receive response to RRQ within %d seconds.\n", DEFAULT_TIMEOUT_SECS);
        exit(1);
      }

      if (errno == EAGAIN || errno == ETIMEDOUT)
      {
        if (retry_count < MAX_RETRIES)
        {
          print_if_verbose("  Timed out, ACKing again. Attempt %d\n", retry_count);
          create_ack(block_num, ack_packet, ack_packet_size);
          send_packet(fd, ack_packet, ack_packet_size, (struct sockaddr *)&incoming_addr, addr_len, "RETRY ACK");

          retry_count++;
          continue;
        }
        else
        {
          print_if_verbose("Max retries reached. Exiting.\n");
          exit(1);
        }
      }
      else
      {
        perror("Receiving packet");
        exit(1);
      }
    }
    print_if_verbose("Received %d bytes from server\n", numbytes);

    recv_buffer[numbytes] = '\0';
    int opcode = recv_buffer[1];

    switch (opcode)
    { // Respond to packet based on what kind it is
    case 1:
      continue;
      break;

    case 2:
      continue;
      break;

    case 3:
    {
      print_if_verbose("- DATA packet\n");
      retry_count = 0;

      if (iter == 0 && options)
      { // DATA packet after options RRQ means server doesn't have option extension
        print_if_verbose("  Server rejected all options, proceeding WITHOUT OPTIONS.\n");
        WINDOWSIZE_OPTION = (int)NULL;
        BLOCKSIZE_OPTION = DEFAULT_BLOCKSIZE;
      }

      if (window_iter >= WINDOWSIZE_OPTION)
      {
        window_iter = 1;
        // Send the ACK

        block_num[0] = recv_buffer[2];
        block_num[1] = recv_buffer[3];

        create_ack(block_num, ack_packet, ack_packet_size);
        send_packet(fd, ack_packet, ack_packet_size, (struct sockaddr *)&incoming_addr, addr_len, "ACK");

        print_if_verbose("  ACKed: %d\n", block_num[1]);
      }
      else
      {
        window_iter++;
      }
      break;
    }
    case 4:
      continue;
      break;

    case 5:
    {
      print_if_verbose("- ERROR packet\n");
      int error_code = recv_buffer[3];
      char error_msg[strlen(recv_buffer + 4)];
      strcpy(error_msg, recv_buffer + 4);

      print_if_verbose("  Error: %s\n  Message: %s\n", ERROR_CODES[error_code], error_msg);
      break;
    }

    case 6:
    {
      if (iter == 0 && options)
      { // Handle options ACK
        is_oack = 1;
        print_if_verbose("- OACK\n");
        if (process_oack(recv_buffer, &BLOCKSIZE_OPTION, &WINDOWSIZE_OPTION) == 0)
        {
          // ACK with block num 0
          print_if_verbose("  options accepted and loaded\n");
          create_ack("\0\0", ack_packet, ack_packet_size);
          send_packet(fd, ack_packet, ack_packet_size, (struct sockaddr *)&incoming_addr, addr_len, "ACK for OACK");

          print_if_verbose("  ACKed: 0\n");
        }
        else
        {
          // Send error code 8 and exit
          char error_packet[error_length];
          create_oack_err(error_packet, error_length);
          send_packet(fd, error_packet, error_length, (struct sockaddr *)&incoming_addr, addr_len, "ERROR");

          exit(1);
        }
      }
      else
      {
        continue;
        break;
      }
      break;
    }
    }

    iter++;
  } while (numbytes == BLOCKSIZE_OPTION + 4 || is_oack || retry_count > 0);

  free(rrq_packet);
  free(ack_packet);

  return 0;
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    print_if_verbose("Bad usage");
    exit(2);
  }

  char *target = argv[1];
  char *port = argv[2];
  char *file = argv[3];

  print_if_verbose("Getting %s from %s:%s\n", file, target, port);
  // struct OPTION myOptions[2];

  // myOptions[0].name = "blksize";
  // myOptions[0].value = "1024";
  // myOptions[0].silent = 0;

  // myOptions[1].name = "windowsize";
  // myOptions[1].value = "4";
  // myOptions[1].silent = 0;
  get(target, port, file, NULL, 0);
}