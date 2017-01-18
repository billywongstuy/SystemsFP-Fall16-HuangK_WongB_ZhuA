#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>

#include "networking.h"
#include "cards.h"
#include "players.h"
#include "printMethods.h"
#include "input.h"
#include "turns.h"
#include "rules.h"


//NEXT
//WORK ON USED PILE
//WORK ON RECENT MOVE
//USER INTERFACE

struct card usedCards[52];
int amountUsed;
struct card lastMove[5];
int lastMoveAmount;
char * lastMoveString;

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};


struct card deck[52];
struct player p1,p2,p3,p4;
struct player *playersM[4];
int *allCardsLeft;

int mode;  //0 for debug, 1 for single, 2 for double, 3 for triple, 5 for 5-combo
char * turnPlayerInfo;
int step;
int *idToPass;

int sems[4];
int sc;
struct sembuf sb;

int turnNumber;

void process( char * s );
void sub_server( int sd );
void step1();
void initialize();
void sendUpdatedVars(int sd, char * buffer);


void incID() {
  (*idToPass) ++;
}

int main() {

  srand(time(NULL));
  
  int sd, connection;

  sd = server_setup();

  //NOW NEED SEMAPHORES IN THIS
  initialize();
  printf("f: %d\n",getFirstPlayer(playersM,4,13));
 
  while (1) {
  
    connection = server_connect( sd );

    int f = fork();
    if ( f == 0 ) {
      
      close(sd);
      
      sub_server( connection );
      exit(0);
    }
    else {
      close( connection );
    }
    
    
    incID();
    
  }
  return 0;
}


void sub_server( int sd ) {
  char buffer[MESSAGE_BUFFER_SIZE];
  
  int first = getFirstPlayer(playersM,4,13);
  setTurnPlayer(first);
  char *start = (char *)malloc(sizeof(char));
  
  //ASSIGNS PLAYER ID
  //IF TOO MANY PLAYERS, WRITES A MESSAGE INSTEAD OF ID
  if (*idToPass < 4) {
    printf("PID: %d\n",*idToPass);
    sprintf(buffer,"%d",*idToPass);
    write(sd,buffer,sizeof(buffer));

    //BLOCKS EVERYONE AT DEFAULT
    sb.sem_op = -1;
    semop(sems[*idToPass],&sb,1);

    //If 4 PLAYERS ARE CONNECTED, UNBLOCKS THE FIRST PLAYER
    if (*idToPass == 3) {
      sb.sem_op = 1;
      semop(sems[first],&sb,1);
    }
    
  }

  //TOO MANY PLAYERS
  else {
    sprintf(buffer,"Player cap exceeded");
    write(sd,buffer,sizeof(buffer));
    exit(0);
  }

  
  //SEMAPHORE OF NEXT
  sprintf(buffer,"%d",sems[nextPlayer(4,*idToPass)]);
  write(sd,buffer,sizeof(buffer));


  //INITIAL PLAYER INFO
  strcpy(start,memPrintPlayerClient(playersM[*idToPass]));
  strcat(start,"  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 |  13 |\n\n" );
  sprintf(buffer,"%s",start);	
  write(sd,buffer,sizeof(buffer));  


  //sendUpdatedVars(sd);

  char varBuffer[1000];
  
  while (1) {


    
    int gtp = getTurnPlayer();
    //write(sd,&gtp,sizeof(gtp));
    //printf("ggggg: %d\n", gtp);
    sprintf(buffer,"%d\n",gtp);
    write(sd,buffer,sizeof(buffer));


    write(sd,"COME ON",16);
    
    //strcpy(buffer,lastMoveString);
    char * lma = getLastMove();
    printf("glm: %s\n",lma);
    strcpy(varBuffer,lma);
    write(sd,varBuffer,sizeof(varBuffer));
    //printf("lm non buf: %s\n",lastMoveString);
    //printf("lm-pre: %s\n",buffer);
    
    int * acl = getAllCardsLeft();
    int j;
    for (j = 0; j < 4; j++) {
      printf("acl: %d\t",acl[j]);
    }
    write(sd,acl,sizeof(acl));



    read( sd, buffer, sizeof(buffer) );
    
    //CLIENT INPUT
    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    //PROCESSED INFO
    write( sd, buffer, sizeof(buffer));    //This is what is passed to client



    //VARIABLES
    gtp = getTurnPlayer();
    write(sd,&gtp,sizeof(gtp));
    
    //printf("glm: %s\n",lastMoveString);
    //strcpy(buffer,lastMoveString);
    char * lms = getLastMove();
    strcpy(varBuffer,lms);
    write(sd,varBuffer,sizeof(varBuffer));
    
    acl = getAllCardsLeft();
    for (j = 0; j < 4; j++) {
      printf("acl[%d]: %d\n",j,acl[j]);
    }
    write(sd,acl,sizeof(acl));
    

    
    //PLAYER INFO
    strcpy(start,memPrintPlayerClient(playersM[*idToPass]));
    strcat(start,"  1   |");

    int i;
    for (i = 0; i < playersM[*idToPass]->cardsLeft-1; i++) {
      if (i >= 8) {sprintf(start + strlen(start),"  %d  |",i+2);}
      else {sprintf(start + strlen(start),"   %d  |",i+2);}
    }
    strcat(start,"\n\n");    
    sprintf(buffer,"%s",start);


    //printf("player info to send:\n%s\n",buffer);

    write(sd,buffer,sizeof(buffer));

    
    
  }
  
}



void process( char * s ) {

  step1(s);
}


void step1(char *s) {

  int count = 0;
  int chosen[5];
  char * in = s;
  
  int len = getInput(chosen,in,playersM[getTurnPlayer()]->cardsLeft);
  int next = nextPlayer(4,getTurnPlayer());

  //INVALID CHOICE
  if (len == 0) {
    strcpy(s,"Invalid selection(s)");
    printf("wrong\n");
    sb.sem_op = 1;
    semop(sems[getTurnPlayer()],&sb,1);
  }
  // PROHIBITED TO USE THE CARD(S)  e.g. wrong mode, too low
  //elseif ...
  else {
    
    struct card selected[len];
    char * error = getCardsChosen(selected,chosen,len,playersM,getTurnPlayer());
    sortCards(selected,len);
    
    /*while (count < len && selected[count].value != -1) {
      printCard(selected[count]);
      count++;
    }*/

    for (count; count < len; count++) {
      lastMove[count] = selected[count];
      
      usedCards[count+amountUsed] = selected[count];
    }
    amountUsed += len;
    lastMoveAmount = len;

    lastMoveString = printChoice(selected,len,getTurnPlayer());
    setLastMove(printChoice(selected,len,getTurnPlayer()));

    printf("lms updated: %s\n",lastMoveString);
    
    useCards(playersM[getTurnPlayer()],chosen,len);
    
    setAllCardsLeft(playersM[getTurnPlayer()]->cardsLeft,getTurnPlayer());
    
    sb.sem_op = 1;
    semop(sems[next],&sb,1);
    
    
    setTurnPlayer(next);
    strcpy(s,"Valid selection(s)");
    
  }
    
}



void initialize() {
  
  playersM[0] = &p1;
  playersM[1] = &p2;
  playersM[2] = &p3;
  playersM[3] = &p4;

  setupDeck(deck);
  
  distributeCards(deck,playersM,4);
  
  sortCards(p1.hand,13);
  sortCards(p2.hand,13);
  sortCards(p3.hand,13);
  sortCards(p4.hand,13);

  idToPass = (int *)malloc(sizeof(int));
  *idToPass = 0;

  amountUsed = 0;

  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  union semun su;
  su.val = 1;
  
  int i;
  for (i = 0; i < 4; i++) {
    sems[i] = semget(ftok("server.c",i),1,IPC_CREAT | 0644);
    printf("semaphore created: %d\n",sems[i]);
    sc = semctl(sems[i],0,SETVAL,su);
    printf("value set: %d\n",sc);
  }

  turnNumber = 0;

  allCardsLeft = (int *)malloc(sizeof(int));
  for (i=0;i<4;i++){allCardsLeft[i]=13;}

  lastMoveString = (char *)malloc(sizeof(char));
  strcpy(lastMoveString,"This is the first move");
  setLastMove("This is the first move");
  
  setAllCardsLeft(13,0);
  setAllCardsLeft(13,1);
  setAllCardsLeft(13,2);
  setAllCardsLeft(13,3);
}


