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


union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};


struct card deck[52];
struct player p1,p2,p3,p4;
struct player *playersM[4];
int mode;  //0 for debug, 1 for single, 2 for double, 3 for triple, 5 for 5-combo
int turnPlayer; //is an index for player who has to make their move
int turnNumber; //use this to check player turn
char * turnPlayerInfo;
int step;
int *idToPass;
//use pointers to access stuff as part of the global

void process( char * s );
void sub_server( int sd );
void step1();
void initialize();


void incID() {
  (*idToPass) ++;
}

int main() {

  srand(time(NULL));
  
  int sd, connection;

  sd = server_setup();

  //NOW NEED SEMAPHORES IN THIS
  initialize();
  
 
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
  char *suits[] = {"♦", "♣", "♥", "♠"};

  memPrintPlayer(playersM[0]);
  memPrintPlayer(playersM[1]);
  memPrintPlayer(playersM[2]);
  memPrintPlayer(playersM[3]);
  
  int first = getFirstPlayer(playersM,4,13);
  setTurnPlayer(first);
  printf("first: %d\n",first);
  char *start = (char *)malloc(sizeof(char));
  
  
  //ASSIGNS PLAYER ID
  //IF TOO MANY PLAYERS, WRITES A MESSAGE INSTEAD OF ID
  if (*idToPass < 4) {
    printf("pid: %d\n",*idToPass);
    sprintf(buffer,"%d",*idToPass);
    write(sd,buffer,sizeof(buffer));
  }
  else {
    sprintf(buffer,"Player cap exceeded");
    write(sd,buffer,sizeof(buffer));
    exit(0);
  }

  //INITIAL PLAYER INFO
  strcpy(start,memPrintPlayerClient(playersM[*idToPass]));
  sprintf(buffer,"%s",start);	
  write(sd,buffer,sizeof(buffer));  
  
  
  while (read( sd, buffer, sizeof(buffer) )) {

    //CLIENT INPUT
    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    printf("processed\n");

    //PLAYER INFO
    strcpy(start,memPrintPlayerClient(playersM[*idToPass]));
    sprintf(buffer,"%s",start);	
    write(sd,buffer,sizeof(buffer));
    read(sd,buffer,sizeof(buffer));

    //PROCESSED INFO
    write( sd, buffer, sizeof(buffer));    //This is what is passed to client
    
  }
  
}



void process( char * s ) {

  step1(s);
}


void step1(char *s) {
  
  int count = 0;

  int chosen[5];


  char * in = s;
  
  int len = getInput(chosen,in);

  
  if (len == 0) {
    s = "Invalid selection(s)\n";
  }

  else {
    
    struct card selected[len];

    int first = getFirstPlayer(playersM,4,13);
    //int first = 0;
    
    char * error = getCardsChosen(selected,chosen,len,playersM,first);
    sortCards(selected,len);
    
    while (count < len && selected[count].value != -1) {
      printCard(selected[count]);
      count++;
    }

    
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

  //ALSO MAKE SURE SETTING FIRST PLAYER WORKS
  //??? BLOCK UNTIL 4 PLAYERS CONNECTED
  int sem;
  int semkey = ftok("server.c",22);
  int sc;
  
  sem = semget(semkey,1,IPC_CREAT | 0644);
  printf("semaphore created: %d\n",sem);

  union semun su;
  su.val = 1;
  sc = semctl(sem,0,SETVAL,su);
  printf("value set: %d\n",sc);
  
  //now figure out blocking in scenarios with 
  
}
