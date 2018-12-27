#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include "client.h"

void create_rrq(char *filename, char *packet, int rrq_packet_size)
{
  memset(packet, 0, rrq_packet_size);

  // strcpy(packet, RRQ_OPCODE_FIELD);
  packet[0] = 0;
  packet[1] = RRQ_OPCODE;

  strncat(&packet[2], filename, strlen(filename));
  strncat(&packet[2 + strlen(filename) + 1], MODE, 8);
  packet[rrq_packet_size - 1] = '\0';
}

void create_ack(char *block_num, char *packet, int ack_packet_size)
{
  memset(packet, 0, ack_packet_size);

  // strncpy(packet, ACK_OPCODE_FIELD, OPCODE_SIZE);

  packet[0] = 0;
  packet[1] = ACK_OPCODE;

  packet[2] = block_num[0];
  packet[3] = block_num[1];
}

int get(char *target, char *port, char *filename)
{
  // Initializing sockets
  struct addrinfo hints, *servinfo, *temp_sock;
  int addrResult;
  int fd;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  struct sockaddr_storage their_addr;
  socklen_t addr_len;
  char recv_buffer[BUFFER_LENGTH];


  // Allocating packet buffers
  size_t rrq_packet_size = 2 + strlen(filename) + 1 + strlen(MODE) + 1;
  char *rrq_packet = malloc(rrq_packet_size);
  size_t ack_packet_size = ACK_SIZE;
  char *ack_packet = malloc(ack_packet_size);

  if ((addrResult = getaddrinfo(target, port, &hints, &servinfo)) != 0)
  {
    printf("Could not get address info.");
    return 1;
  }

  for (temp_sock = servinfo; temp_sock != NULL; temp_sock = temp_sock->ai_next)
  {
    if ((fd = socket(temp_sock->ai_family, temp_sock->ai_socktype, temp_sock->ai_protocol)) == -1)
      continue;
    break;
  }

  if (temp_sock == NULL)
  {
    printf("Could not create a socket with those DNS addresses.");
    return 2;
  }

  int numbytes;
  int acknumbytes;

  // create RRQ
  create_rrq(filename, rrq_packet, rrq_packet_size);

  // send out RRQ
  if ((numbytes = sendto(fd, rrq_packet, rrq_packet_size, 0, temp_sock->ai_addr, temp_sock->ai_addrlen)) == -1)
  {
    perror("Sending RRQ");
    exit(1);
  }
  printf("Sent %d bytes to %s on port %s\n", numbytes, target, port);

  // loop: listen for DATA, send ACK
  do
  {
    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(fd, recv_buffer, BUFFER_LENGTH - 1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1)
    {
      perror("Receiving packet");
      exit(1);
    }
    recv_buffer[numbytes] = '\0';

    printf("Received %d bytes from server\n", numbytes);

    int opcode = recv_buffer[1];

    switch (opcode)
    {
    case 1:
      continue;
      break;

    case 2:
      continue;
      break;

    case 3:
      printf("- DATA packet\n");

      // Send the ACK
      char block_num[2];
      block_num[0] = recv_buffer[2];
      block_num[1] = recv_buffer[3];

      create_ack(block_num, ack_packet, ack_packet_size);
      if ((acknumbytes = sendto(fd, ack_packet, ack_packet_size, 0, (struct sockaddr *)&their_addr, addr_len)) == -1)
      {
        perror("Sending ACK");
        exit(1);
      }
      printf("ACKed: %d\n", block_num[1]);
      break;

    case 4:
      continue;
      break;

    case 5:
      printf("- ERROR packet\n");
      int error_code = recv_buffer[3];
      char error_msg[strlen(recv_buffer+4)];
      strcpy(error_msg, recv_buffer+4);

      printf("  Error: %s\n  Message: %s\n", ERROR_CODES[error_code], error_msg);
      break;
    }

  } while (numbytes == 516);

  free(rrq_packet);
  free(ack_packet);

  return 0;
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    printf("Bad usage");
    return 2;
  }

  char *target = argv[1];
  char *port = argv[2];
  char *file = argv[3];

  printf("Getting %s from %s:%s\n", file, target, port);

  get(target, port, file);
}
