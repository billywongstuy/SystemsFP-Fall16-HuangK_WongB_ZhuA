#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"
#include "printMethods.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>


int playerId;

int main( int argc, char *argv[] ) {
  
  char *host;

  if (argc != 2 ) {
    printf("host not specified, connecting to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];
  
  int sd;
  
  sd = client_connect( host );

  char buffer[MESSAGE_BUFFER_SIZE];


  //Gets the playerId
  read( sd, buffer, sizeof(buffer));

  if (strcmp(buffer,"Player cap exceeded") == 0) {
    printf("%s\n",buffer);
    exit(0);
  }
  
  playerId = atoi(buffer);
  printf("pid: %d\n",playerId);
  
  //Gets the card info
  read( sd, buffer, sizeof(buffer) ); //This is what the server gets
  printf( "received: %s\n", buffer);
  
  
  while (1) {
    printf("Choose your card(s): ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;
    
    write( sd, buffer, sizeof(buffer) ); //This is what is passed to server

    
    read( sd, buffer, sizeof(buffer) ); //This is what the server gets
    
    printf( "received: %s\n", buffer);
  }
  
  return 0;
}
