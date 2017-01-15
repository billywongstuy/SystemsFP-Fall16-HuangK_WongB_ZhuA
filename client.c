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

  //SEMAPHORE
  int sem;
  int semkey = ftok("server.c",22);
  sem = semget(semkey,1,0);
  printf("semaphore created: %d\n",sem);
  
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
  

  //INITIAL PLAYER INFO
  read( sd, buffer, sizeof(buffer) ); //This is what the server gets
  printf( "received: %s\n", buffer);

  
  while (1) {

    //NEED TO FIX FIGURE OUT HOW TO BLOCK IF NOT PLAYER TURN
    //NEED TO FIGURE HOW TO NOT BLOCK IF INVALID INPUT
    printf("first player: %d\n",getTurnPlayer());
      
    
    printf("Choose your card(s): ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;    
    write( sd, buffer, sizeof(buffer) ); //This is what is passed to server


    //PROCESSED RESULT
    read( sd, buffer, sizeof(buffer) ); //This is what the server gets
    printf( "received: %s\n", buffer);
    
    //PLAYER INFO
    read( sd, buffer, sizeof(buffer) ); //This is what the server gets
    printf( "received: %s\n", buffer);


    printf("unblock\n");
    sb.sem_op = 1;
    semop(sem,&sb,1);
    
  }
  
  return 0;
}



