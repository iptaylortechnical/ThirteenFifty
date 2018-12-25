#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "client.h"

int grandom(int min, int max)
{
  return min + (rand() % (max - min));
}

char *create_rrq(char *filename)
{
  char *packet;
  packet = malloc(2 + strlen(filename) + 1 + strlen(MODE));
  memset(packet, 0, sizeof packet);
  strcat(packet, RRQ);
  strcat(packet, filename);
  return packet;
}

char *create_ack(char *block_num)
{
  char *packet;
  packet = malloc(2 + strlen(block_num));
  memset(packet, 0, sizeof packet);
  strcat(packet, "04"); //opcode
  strcat(packet, block_num);
  return packet;
}

int get(char *target, char *filename)
{
  // init
  // struct addrinfo hints, *servinfo;
  struct addrinfo hints, *servinfo, *temp_sock;
  int addrResult;
  int fd;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  struct sockaddr_storage their_addr;
  socklen_t addr_len;
  char recv_buffer[BUFFER_LENGTH];

  char tID[6];
  srand(time(NULL));

  if ((addrResult = getaddrinfo(target, INIT_PORT, &hints, &servinfo)) != 0)
  {
    printf("failed at 1");
    return 1;
  }

  for (temp_sock = servinfo; temp_sock != NULL; temp_sock->ai_next)
  {
    if ((fd = socket(temp_sock->ai_family, temp_sock->ai_socktype, temp_sock->ai_protocol)) == -1)
      continue;
    break;
  }

  if (temp_sock == NULL)
  {
    printf("failed at 2");
    return 2;
  }

  // do
  // {
  //   sprintf(tID, "%d", grandom(1024, 65535));
  //   printf(tID);

  //   if ((addrResult = getaddrinfo(target, tID, &hints, &servinfo)) != 0)
  //   {
  //     continue;
  //   }

  //   for (temp_sock = servinfo; temp_sock != NULL; temp_sock->ai_next)
  //   {
  //     if ((fd = socket(temp_sock->ai_family, temp_sock->ai_socktype, temp_sock->ai_protocol)) == -1) continue;
  //     break;
  //   }

  //   if (temp_sock == NULL) {
  //     continue;
  //   }

  //   break;
  // } while (1);

  printf("%d", fd);
  printf("\n");
  int numbytes;
  int acknumbytes;

  // create RRQ

  char *msg = create_rrq(filename);

  if ((numbytes = sendto(fd, msg, strlen(msg), 0, temp_sock->ai_addr, temp_sock->ai_addrlen)) == -1)
  {
    perror("CLIENT: sendto");
    exit(1);
  }
  printf("CLIENT: sent %d bytes to %s\n", numbytes, target);

  // send out RRQ

  // loop: listen for DATA, send ACK, each time check for
  do
  {
    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(fd, recv_buffer, BUFFER_LENGTH - 1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1)
    {
      perror("CLIENT: recvfrom");
      exit(1);
    }

    printf("Received %d bytes from server\n", numbytes);

    recv_buffer[numbytes] = '\0';

    // ACK

    char block_num[3];
    strncpy(block_num, recv_buffer + 2, 2);
    block_num[2] = '\0';

    char *ack = create_ack(block_num);

    if ((acknumbytes = sendto(fd, ack, strlen(ack), 0, temp_sock->ai_addr, temp_sock->ai_addrlen)) == -1)
    {
      perror("CLIENT ACK: sendto");
      exit(1);
    }
    printf("acked");

  } while (numbytes > 512);

  return 0;
}

int main()
{
  get("localhost", "utility.h");
}
