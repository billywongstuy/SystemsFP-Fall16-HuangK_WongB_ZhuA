#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>


#include "networking.h"
#include "printMethods.h"

/*
struct card deck[52];
struct player p1,p2,p3,p4;
struct player *players[4];
int mode;  //0 for debug, 1 for single, 2 for double, 3 for triple, 5 for 5-combo
int turnPlayer; //is an index for player who has to make their move
int turnNumber; //use this to check player turn
char * turnPlayerInfo;
int step;
*/

void error_check( int i, char *s ) {
  if ( i < 0 ) {
    printf("%d\n", i);
    printf("[%s] error %d: %s\n", s, errno, strerror(errno) );
    exit(1);
  }
}

int server_setup() {
  
  int sd;
  int i;
  
  sd = socket( AF_INET, SOCK_STREAM, 0 );
  error_check( sd, "server socket" );
  
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  sock.sin_addr.s_addr = INADDR_ANY;
  sock.sin_port = htons(9001);
  i = bind( sd, (struct sockaddr *)&sock, sizeof(sock) );
  error_check( i, "server bind" );
  
  return sd;
}

int server_connect(int sd) {
  int connection, i;

  i = listen(sd, 1);
  error_check( i, "server listen" );
  
  struct sockaddr_in sock1;
  unsigned int sock1_len = sizeof(sock1);
  connection = accept( sd, (struct sockaddr *)&sock1, &sock1_len );
  error_check( connection, "server accept" );
  
  printf("[server] connected to %s\n", inet_ntoa( sock1.sin_addr ) );
  
  return connection;
}


int client_connect( char *host ) {
  int sd, i;
  
  sd = socket( AF_INET, SOCK_STREAM, 0 );
  error_check( sd, "client socket" );
  
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;
  inet_aton( host, &(sock.sin_addr));
  sock.sin_port = htons(9001);
  
  printf("[client] connecting to: %s\n", host );
  i = connect( sd, (struct sockaddr *)&sock, sizeof(sock) );
  error_check( i, "client connect");
  
  return sd;
}



void setup() {

  
  int shmem;
  int shmemkey = ftok("cards.c",40);
  
  shmem = shmget(shmemkey, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);

  if (shmem == -1) {
    printf("shmem error: %s\n",strerror(errno));
  }
  int *size = shmat(shmem,NULL,0);

  if (*size == -1) {
    printf("shmat error: %s\n",strerror(errno));
  }

  *size = 45;

  /*
  int sDeck,sPlay,sMode,sTP,sTN,sTPI,sStep;
  int sDeckKey = ftok("cards.c",22);
  int sPlayKey = ftok("cards.c",23);
  int sModeKey = ftok("cards.c",24);
  int sTPKey = ftok("cards.c",25);
  int sTNKey = ftok("cards.c",26);
  int sTPIKey = ftok("cards.c",27);
  int sStepKey = ftok("cards.c",28);

  sDeck = shmget(sDeckKey, sizeof(deck), IPC_CREAT | IPC_EXCL | 0644);
  sPlay = shmget(sPlayKey, sizeof(players), IPC_CREAT | IPC_EXCL | 0644);
  sMode = shmget(sModeKey, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
  sTP = shmget(sTPKey, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
  sTN = shmget(sTNKey, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
  sTPI = shmget(sTPIKey, 1000, IPC_CREAT | IPC_EXCL | 0644);
  sStep = shmget(sStepKey, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);

  struct card (*deckM)[52];
  deckM = shmat(sDeck,NULL,0);

  struct player (* *playersM)[4];
  playersM  = shmat(sPlay,NULL,0);

  int *modeM = shmat(sMode,NULL,0);
  int *turnPlayerM = shmat(sTP,NULL,0);
  char ** turnPlayerInfoM = shmat(sTPI,NULL,0);
  int *stepM = shmat(sStep,NULL,0);
  */

  
  /*
  players[0] = &p1;
  players[1] = &p2;
  players[2] = &p3;
  players[3] = &p4;

  setupDeck(deck);
  
  distributeCards(deck,players,4);
  
  sortCards(p1.hand,13);
  sortCards(p2.hand,13);
  sortCards(p3.hand,13);
  sortCards(p4.hand,13);
 */

  
  /*
  *stepM = 45;
  *turnPlayerM = 0;

  deckM = &deck;
  playersM = &players;
  */

  
}

/*
char * getHand(struct player p1) {
  return printPlayerClient(p1);
}


struct card * getDeck() {return deck;}
struct player getp1() {return p1;}
struct player getp2() {return p2;}
struct player getp3() {return p3;}
struct player getp4() {return p4;}
struct player ** getPlayers() {return players;}
int getMode() {return mode;}
int getTurnPlayer() {return turnPlayer;}
int getTurnNumber() {return turnNumber;}
char * getTurnPlayerInfo() {return turnPlayerInfo;}
int getStep() {return step;}
*/
