#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"
#include "printMethods.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>


int playerId;


int main( int argc, char *argv[] ) {

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  
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
  
  //GETS PLAYER ID
  read( sd, buffer, sizeof(buffer));

  //IF TOO MANY PLAYERS EXITS CLIENT
  if (strcmp(buffer,"Player cap exceeded") == 0) {
    printf("%s\n",buffer);
    exit(0);
  }
  
  //PLAYER ID SET
  playerId = atoi(buffer);
  printf("pid: %d\n",playerId);
  

  //SEMAPHORE
  int sem;
  int semkey = ftok("server.c",playerId);
  sem = semget(semkey,1,0);
  printf("semaphore created: %d\n",sem);

  printf("\033c");


  //SEMAPHORE OF NEXT
  read(sd,buffer,sizeof(buffer));
  int nextSem = atoi(buffer);
  //printf("nextsem: %d\n",nextSem);

  //INITIAL PLAYER INFO
  read( sd, buffer, sizeof(buffer) ); //This is what the server gets
  printf( "%s", buffer);
  printf("Waiting for your turn...\n");

  while (1) {

    sb.sem_op = -1;
    semop(sem,&sb,1);
    //printf("blocking %d\n",sem);  
    
    printf("\033c");

    printf("Last Move: %s\n\n",getLastMove());

    int * racl = getAllCardsLeft();
    int k;
    for (k = 0; k < 4; k++) {
      if (k != playerId) {printf("Player %d Cards Left: %d\n",k+1,racl[k]);}
    }
    
    printf("%s",buffer);


    printf("Choose your card(s): ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;    
    write( sd, buffer, sizeof(buffer) ); //This is what is passed to server

    printf("\033c");

    //PROCESSED RESULT
    read( sd, buffer, sizeof(buffer) ); //This is what the server gets
    printf( "received: %s\n", buffer);


    //PLAYER INFO
    read( sd, buffer, sizeof(buffer) ); //This is what the server gets
    printf( "%s", buffer);


    if (strcmp(buffer,"Invalid selection(s)") != 0) {
      sb.sem_op = 1;
      semop(nextSem,&sb,1);
      //printf("unblocking %d\n",nextSem);
    }
   

  }
  
  return 0;
}



