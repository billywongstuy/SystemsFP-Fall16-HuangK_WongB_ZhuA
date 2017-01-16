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
int sem;
int semkey;

int sems[4];

int sc;
struct sembuf sb;
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

  //INITIAL PLAYER INFO
  strcpy(start,memPrintPlayerClient(playersM[*idToPass]));
  sprintf(buffer,"%s",start);	
  write(sd,buffer,sizeof(buffer));  


  //SEMAPHORE OF NEXT
    sprintf(buffer,"%d",sems[nextPlayer(4,*idToPass)]);
    write(sd,buffer,sizeof(buffer));
  
  while (read( sd, buffer, sizeof(buffer) )) {
    
    //CLIENT INPUT
    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );

    //PROCESSED INFO
    write( sd, buffer, sizeof(buffer));    //This is what is passed to client
    
    //PLAYER INFO
    strcpy(start,memPrintPlayerClient(playersM[*idToPass]));
    sprintf(buffer,"%s",start);	
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
    useCards(playersM[getTurnPlayer()],chosen,len);
    
    setTurnPlayer(next);
    
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

  
  //??? BLOCK UNTIL 4 PLAYERS CONNECTED
  semkey = ftok("server.c",22);
   
  sem = semget(semkey,1,IPC_CREAT | 0644);
  printf("semaphore created: %d\n",sem);


  //-------------------------
  //ABOVE IS SINGULAR

  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  
  union semun su;
  su.val = 1;
  sc = semctl(sem,0,SETVAL,su);
  printf("value set: %d\n",sc);

  
  int i;
  for (i = 0; i < 4; i++) {
    sems[i] = semget(ftok("server.c",i),1,IPC_CREAT | 0644);
    printf("semaphore created: %d\n",sems[i]);
    sc = semctl(sems[i],0,SETVAL,su);
    printf("value set: %d\n",sc);
  }

  
}
