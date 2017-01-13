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

  //print the player hand
  //printPlayer(getp1());


  //SHARED MEMORY

  int shmem;
  int shmemkey = ftok("cards.c",40);
  
  shmem = shmget(shmemkey, sizeof(int), 0644);

  if (shmem == -1) {
    printf("shememor error: %s\n",strerror(errno));
  }

  int *size = shmat(shmem,NULL,0);

  if (*size == -1) {
    printf("shmat error: %s\n",strerror(errno));
  }

  printf("size: %d\n",*size);


  int sh;
  sh = shmctl(shmem,IPC_RMID, NULL);
  printf("shared memory removed: %d\n",sh);


  //------------------------------

  /*
  int sDeck,sPlay,sMode,sTP,sTN,sTPI,sStep;
  
  int sDeckKey = ftok("cards.c",22);
  int sPlayKey = ftok("cards.c",23);
  int sModeKey = ftok("cards.c",24);
  int sTPKey = ftok("cards.c",25);
  int sTNKey = ftok("cards.c",26);
  int sTPIKey = ftok("cards.c",27);
  int sStepKey = ftok("cards.c",28);
  
  struct card (*deckM)[52];
  deckM = shmat(sDeck,NULL,0);
  
  struct player (* *playersM)[4];
  playersM = shmat(sPlay,NULL,0);
  
  int *modeM = shmat(sMode,NULL,0);
  int *turnPlayerM = shmat(sTP,NULL,0);
  char ** turnPlayerInfoM = shmat(sTPI,NULL,0);
  int *stepM = shmat(sStep,NULL,0);
  

  printf("tp: %d\n",*turnPlayerM);
  //printf("players: %s\n",*playersM);
  
  int sh2;
  sh2 = shmctl(sDeck,IPC_RMID, NULL);
  printf("shared memory removed: %d\n",sh2);
  sh2 = shmctl(sPlay,IPC_RMID, NULL);
  printf("shared memory removed: %d\n",sh2);
  sh2 = shmctl(sMode,IPC_RMID, NULL);
  printf("shared memory removed: %d\n",sh2);
  sh2 = shmctl(sTP,IPC_RMID, NULL);
  printf("shared memory removed: %d\n",sh2);
  sh2 = shmctl(sTPI,IPC_RMID, NULL);
  printf("shared memory removed: %d\n",sh2);
  sh2 = shmctl(sStep,IPC_RMID, NULL);
  printf("shared memory removed: %d\n",sh2);
*/
  //END SHARED MEMORY
  printf("endo\n");

  while (1) {
    printf("Choose your card(s): ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;
  
    write( sd, buffer, sizeof(buffer) ); //This is what is passed to server
    read( sd, buffer, sizeof(buffer) ); //This is what the server gets
    printf( "received: %s\n", buffer );
  }
  
  return 0;
}
