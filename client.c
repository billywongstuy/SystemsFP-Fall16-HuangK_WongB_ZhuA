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
    printf("host not specified, conneting to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];
  
  int sd;
  
  sd = client_connect( host );

  char buffer[MESSAGE_BUFFER_SIZE];



  //SHARED MEMORY
  /*int shmem;
  int shmemkey = ftok("cards.c",40);
  
  shmem = shmget(shmemkey, sizeof(int), 0644);

  if (shmem == -1) {printf("shememor error: %s\n",strerror(errno));}

  int *pID = shmat(shmem,NULL,0);

  if (*pID == -1) {printf("shmat error: %s\n",strerror(errno));}

  *playerId = *pID;
  printf("pid: %d\n",*playerId);
  
  if (*pID == 3) {
    int sh;
    sh = shmctl(shmem,IPC_RMID, NULL);
    printf("shared memory removed: %d\n",sh);
  }*/
  
  //END SHARED MEMORY


  //Gets the playerId
  read( sd, buffer, sizeof(buffer));

  if (strcmp(buffer,"Player cap exceeded") == 0) {
    printf("%s\n",buffer);
    exit(0);
  }
  
  printf("pid: %s\n",buffer);
  playerId = atoi(buffer);
  
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
