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


//WORK ON
//Better Organization and Order for Printing
//Update the last move everytime a player moves

int playerId;
char message[500];
int turnPlayer;
char lastMove[500];
int * allCardsLeft; //int allCardsLeft[4];

void getUpdatedVars(int sd);


int main( int argc, char *argv[] ) {

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;

  //message = (char *)malloc(sizeof(char));
  //lastMove = (char *)malloc(sizeof(char));
  allCardsLeft = (int *)malloc(sizeof(int));
  
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
  char handBuf[MESSAGE_BUFFER_SIZE];
  
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

  //Initial cards left
  int k;
  for (k = 0; k < 4; k++) {
    allCardsLeft[k] = 13;
    if (k != playerId) {printf("Player %d Cards Left: %d\n",k+1,allCardsLeft[k]);}
  }

  
  //INITIAL PLAYER INFO
  read( sd, handBuf, sizeof(handBuf) ); //This is what the server sends
  printf( "%s", handBuf);
  printf("Waiting for your turn...\n");

  

  while (1) {

    //printf("blocking %d\n",sem);  
    //sb.sem_op = -1;
    //semop(sem,&sb,1);


    write(sd,"Turn start",10);
    
    printf("\033c"); //CLEAR

    if (strlen(message) > 0) {
      printf("received; %s\n",message);
      strcpy(message,"");
    }

    //UPDATING VARIABLES BEFORE CHOOSING
    
    read(sd,&turnPlayer,sizeof(turnPlayer));

    read(sd,buffer,sizeof(buffer));
    strcpy(lastMove,buffer);
    
    for (k = 0; k < 4; k++) {
      read(sd,&allCardsLeft[k],sizeof(allCardsLeft[k]));
    }
    //END VARIABLE UPDATE

    
    printf("Last Move: %s\n\n",lastMove);
    
    for (k = 0; k < 4; k++) {
      if (k != playerId) {printf("Player %d Cards Left: %d\n",k+1,allCardsLeft[k]);}
    }
    
    printf("%s",handBuf);

    printf("If it's not your turn, press enter to check if it is your turn\n");
    printf("If it's your turn, choose your card(s):");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;

    write(sd,&playerId,sizeof(playerId));
    
    write( sd, buffer, sizeof(buffer) ); //This is what is passed to server
    
    printf("\033c");  //CLEAR
    
    //PROCESSED RESULT
    read( sd, buffer, sizeof(buffer) ); //This is what the server gets
    strcpy(message,buffer);
    printf( "received-a: %s\n", message);


    //VARIABLES ROUND 2
  
    read(sd,&turnPlayer,sizeof(int));
        
    read(sd,buffer,sizeof(buffer));
    strcpy(lastMove,buffer);
        
    //read(sd,allCardsLeft,sizeof(allCardsLeft));
    for (k = 0; k < 4; k++) {
      read(sd,&allCardsLeft[k],sizeof(allCardsLeft[k]));
    }
    //END VARS R2
    
    
    //WAITING FOR TURN TEXT
    printf("Last Move: %s\n\n",lastMove);
    
    for (k = 0; k < 4; k++) {
      if (k != playerId) {printf("Player %d Cards Left: %d\n",k+1,allCardsLeft[k]);}
    }

    //PLAYER INFO
    read( sd, handBuf, sizeof(handBuf) ); //This is what the server gets
    printf( "Info: %s", handBuf);

    printf("Waiting for your turn...\n");    
    

  }
  
  return 0;
}



void getUpdatedVars(int sd) {
  /*
      int turnPlayer;
      char * lastMove;
      int allCardsLeft[4];
    */

  char buffer[1000];
  int k;

  printf("update time\n");
  
  read(sd,&turnPlayer,sizeof(int));
  printf("tp: %d\n",turnPlayer);
  
  read(sd,buffer,sizeof(buffer));
  strcpy(lastMove,buffer);
  printf("lm: %s\n",buffer);
  
  read(sd,allCardsLeft,sizeof(allCardsLeft));
  for (k = 0; k < 4; k++) {
    printf("cl: %d\n",allCardsLeft[k]);
  }
}
