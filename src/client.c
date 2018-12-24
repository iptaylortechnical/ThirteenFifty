#include <stdio.h>
#include <stdlib.h>
#include "client.h"

int random(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int client(char* target, char* filename) {
  // choose TID

  int tID = random(1024, 65535);
  


  // create RRQ




  // send out RRQ




  // loop: listen for DATA, send ACK, each time check for 
}