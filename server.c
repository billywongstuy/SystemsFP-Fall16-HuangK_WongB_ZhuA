#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>

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

//struct card usedCards[52];
//int amountUsed;
//struct card lastMove[5];
//int lastMoveAmount;
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

  turnNumber = 1;
  initialize();
  setup(); //SHARED MEMORY

  printf("first player id: %d\n",getFirstPlayer(playersM,4,13));

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

    turnNumber++;
    incID();

  }
  return 0;
}


void sub_server( int sd ) {
  char buffer[MESSAGE_BUFFER_SIZE];
  char varBuffer[1000];

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
    //sb.sem_op = -1;
    //semop(sems[*idToPass],&sb,1);

    //If 4 PLAYERS ARE CONNECTED, UNBLOCKS THE FIRST PLAYER
    if (*idToPass == 3) {
      //sb.sem_op = 1;
      //semop(sems[first],&sb,1);
    }

  }

  //TOO MANY PLAYERS
  else {
    sprintf(buffer,"Player cap exceeded");
    write(sd,buffer,sizeof(buffer));
    exit(0);
  }


  //SEMAPHORE OF NEXT
  //sprintf(buffer,"%d",sems[nextPlayer(4,*idToPass)]);
  //write(sd,buffer,sizeof(buffer));


  //INITIAL PLAYER INFO
  strcpy(start,memPrintPlayerClient(playersM[*idToPass]));
  strcat(start,"  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 |  13 |\n\n" );
  sprintf(buffer,"%s",start);
  write(sd,buffer,sizeof(buffer));


  while (1) {

    read(sd,buffer,sizeof(buffer)); //THIS IS TO BLOCK

    //VARIABLES P1
    int gtp = getTurnPlayer();
    write(sd,&gtp,sizeof(gtp));

    char * lma = getLastMove();          strcpy(varBuffer,lma);
    write(sd,varBuffer,sizeof(varBuffer));

    int * acl = getAllCardsLeft();         int j;
    for (j = 0; j < 4; j++) {write(sd,&acl[j],sizeof(acl[j]));}
    //VARS END


    int movePlayer = -1;

    read(sd,&movePlayer,sizeof(movePlayer));
    read( sd, buffer, sizeof(buffer) ); //LISTEN FOR CARD(S) CHOICE

    //CLIENT INPUT
    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    if (getTurnPlayer() == movePlayer) {
      process( buffer );
    }
    else {
      strcpy(buffer,"\x1B[31mIt's not your turn!!!\x1B[0m\n");
    }

    //PROCESSED INFO
    write( sd, buffer, sizeof(buffer)); //PASSED TO CLIENT

    //VARIABLES
    gtp = getTurnPlayer();
    write(sd,&gtp,sizeof(gtp));

    char * lms = getLastMove();       strcpy(varBuffer,lms);
    write(sd,varBuffer,sizeof(varBuffer));

    acl = getAllCardsLeft();
    for (j = 0; j < 4; j++) {write(sd,&acl[j],sizeof(acl[j]));}
    //VARS R2 END


    //PLAYER INFO
    strcpy(start,memPrintPlayerClient(playersM[*idToPass]));
    strcat(start,"  1  |");

    int i;
    for (i = 0; i < playersM[*idToPass]->cardsLeft-1; i++) {
      if (i >= 8) {sprintf(start + strlen(start),"  %d |",i+2);}
      else {sprintf(start + strlen(start),"  %d  |",i+2);}
    }
    strcat(start,"\n\n");
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

  printf("mode: %d\n",getMode());
  printf("len: %d\n",len);

  //INVALID CHOICE
  if (len == 0 && toupper(in[0]) != 80) {
    strcpy(s,"\x1B[31mInvalid selection(s)\x1B[0m\n");
    sb.sem_op = 1;
    semop(sems[getTurnPlayer()],&sb,1);
  }
  else if (getMode() == -1 && strlen(in) == 1 && toupper(in[0]) == 80) {
    strcpy(s,"\x1B[31mNO PASSING ON THE FIRST TURN...\x1B[0m\n");
  }
  else if (strlen(in) == 1 && toupper(in[0]) == 80 && getMode() != -1) {
    setTurnPlayer(next);
    setTurnNumber();
    strcpy(s,"\x1B[36mYou passed\n\x1B[0m");

    char * added = (char *)malloc(sizeof(getLastMove()) + 21);
    strcpy(added,getLastMove());
    strcat(added,"\x1B[36m (Last player passed)\x1B[0m\n");
    setLastMove(added);

    //pass count
    incFreebieNo();
    if (getFreebieNo() == 3) {
      setMode(0);
      char b[500];
      sprintf(b,"\x1B[36mPlayer %d gets a freebie\x1B[0m\n\n",next+1);
      setLastMove(b);
    }
  }
  else if (getMode() != len && getMode() > 0) {
    strcpy(s,"\x1B[31mWrong amount of cards\x1B[0m\n");
  }
  //VALID CHOICE
  else {

    struct card selected[len];
    char * error = getCardsChosen(selected,chosen,len,playersM,getTurnPlayer());
    sortCards(selected,len);


    //MAKE ARRAY TO SEND TO RULES TO CHECK
    int cardValues[len];
    for (count = 0; count < len; count++) {
      struct card curr = selected[count];
      printf("oks: %d %d\n",curr.value*10,curr.suit);
      cardValues[count] = (curr.value*10)+curr.suit;
      printf("%d     ",(curr.value*10)+curr.suit);
    }

    int allowed = validMove(getTurnNumber(),len,getLastAmount(),cardValues,getLastCards());


    //if freebie counter == 3 then ignore the rules
    //afterwards set freebie counter to 0
    //NEED TO INCORPORATE FREEBIES

    printf("allowed: %d\n",allowed);
    printf("get freebie no: %d\n",getFreebieNo());

    if (allowed != 0 && getMode() != 0 && getFreebieNo() != 3) {
      printf("not allowed\n");
      strcpy(s,getInvalidMessage(allowed));
    }
    //sucess
    else {

      resetFreebieNo();

      //SET THE STUFF
      setTurnNumber();

      setLastCards(cardValues,len);
      setLastAmount(len);

      setUsedCards(cardValues,len);
      setUsedAmount(len);

      setMode(len);

      //UPDATING VARIABLES IN SHM
      lastMoveString = printChoice(selected,len,getTurnPlayer());
      setLastMove(printChoice(selected,len,getTurnPlayer()));

      useCards(playersM[getTurnPlayer()],chosen,len);

      setAllCardsLeft(playersM[getTurnPlayer()]->cardsLeft,getTurnPlayer());

      //SET UP THE NEXT PLAYER
      //sb.sem_op = 1;
      //semop(sems[next],&sb,1);

      if (playersM[getTurnPlayer()]->cardsLeft == 0) {
	strcpy(s,"\x1B[33mYOU WON!!!!!!!!!!!!!!!!!\x1B[0m\n");
	char a[500];
	sprintf(a,"\x1B[33mGAME OVER!!! PLAYER %d HAS WON!!!\x1B[0m\n\n",getTurnPlayer()+1);
	setLastMove(a);
      }
      else {
	setTurnPlayer(next);
	strcpy(s,"\x1B[32mValid selection(s)\x1B[0m\n");
      }
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

  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  union semun su;
  su.val = 1;

  int i;
  /*for (i = 0; i < 4; i++) {
    sems[i] = semget(ftok("server.c",i),1,IPC_CREAT | 0644);
    printf("semaphore created: %d\n",sems[i]);
    sc = semctl(sems[i],0,SETVAL,su);
    printf("value set: %d\n",sc);
    }*/

  turnNumber = 0;

  allCardsLeft = (int *)malloc(sizeof(int));
  for (i=0;i<4;i++){allCardsLeft[i]=13;}

  lastMoveString = (char *)malloc(sizeof(char));
  strcpy(lastMoveString,"This is the first move\n\n");
  setLastMove("This is the first move\n\n");

  setAllCardsLeft(13,0);
  setAllCardsLeft(13,1);
  setAllCardsLeft(13,2);
  setAllCardsLeft(13,3);
}
