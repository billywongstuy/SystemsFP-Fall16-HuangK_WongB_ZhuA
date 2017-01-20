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
  shmem = shmget(shmemkey, sizeof(int), IPC_CREAT | 0644);
  if (shmem == -1) {printf("shmem error: %s\n",strerror(errno));}
  int *turnP = shmat(shmem,NULL,0);
  if (*turnP == -1) {printf("shmat error: %s\n",strerror(errno));}
  *turnP = 0;


  int shmem2;
  int shmemkey2 = ftok("cards.c",83);  
  shmem2 = shmget(shmemkey2, sizeof(int), IPC_CREAT | 0644);
  if (shmem2 == -1) {printf("shmem2 error: %s\n",strerror(errno));}
  char (*lastMove)[500];
  lastMove = shmat(shmem2,NULL,0);

  
  int shmem3;
  int shmemkey3 = ftok("cards.c",84);
  shmem3 = shmget(shmemkey3, sizeof(int)*4, IPC_CREAT | 0644);
  if (shmem3 == -1) {printf("shmem3 error: %s\n",strerror(errno));}
  int *a,*b,*c,*d;
  a = shmat(shmem3,NULL,0);
  b = a+1;
  c = b+1;
  d = c+1;

  int okmem;
  int okmemkey = ftok("cards.c",85);
  okmem = shmget(okmemkey,sizeof(int),IPC_CREAT | 0644);
  int *ok = shmat(okmem,NULL,0);
  *ok = 0;
}


int getTurnPlayer() {
  int shmem;
  int shmemkey = ftok("cards.c",40);
  shmem = shmget(shmemkey, sizeof(int), IPC_CREAT | 0644);
  if (shmem == -1) {printf("shmem error: %s\n",strerror(errno));}
  int *turnP = shmat(shmem,NULL,0);
  if (*turnP == -1) {printf("shmat error: %s\n",strerror(errno));}
  return *turnP;
}


void setTurnPlayer(int n) {
  int shmem;
  int shmemkey = ftok("cards.c",40);
  shmem = shmget(shmemkey, sizeof(int), IPC_CREAT | 0644);
  if (shmem == -1) {printf("shmem error: %s\n",strerror(errno));}
  int *turnP = shmat(shmem,NULL,0);
  if (*turnP == -1) {printf("shmat error: %s\n",strerror(errno));}
  *turnP = n;
}


char * getLastMove() {
  int shmem;
  int shmemkey = ftok("cards.c",83);
  shmem = shmget(shmemkey, sizeof(int), IPC_CREAT | 0644);
  if (shmem == -1) {printf("shmem error: %s\n",strerror(errno));}
  char (*lastMove)[500];
  lastMove = shmat(shmem,NULL,0);
  //if (*turnP == -1) {printf("shmat error: %s\n",strerror(errno));}
  return *lastMove;
}


void setLastMove(char * string) {
  int shmem;
  int shmemkey = ftok("cards.c",83);
  shmem = shmget(shmemkey, sizeof(int), IPC_CREAT | 0644);
  if (shmem == -1) {printf("shmem error: %s\n",strerror(errno));}
  char (*lastMove)[500];
  lastMove = shmat(shmem,NULL,0);
  //if (*turnP == -1) {printf("shmat error: %s\n",strerror(errno));}
  strcpy(*lastMove,string);
}



int * getAllCardsLeft() {
  int shmem;
  int shmemkey = ftok("cards.c",84);
  shmem = shmget(shmemkey, sizeof(int)*4, IPC_CREAT | 0644);
  if (shmem == -1) {printf("shmem error: %s\n",strerror(errno));}
  int *a,*b,*c,*d;
  a = shmat(shmem,NULL,0);
  b = a+1;
  c = b+1;
  d = c+1;
  return a;
}


void setAllCardsLeft(int newC, int index) {
  int shmem;
  int shmemkey = ftok("cards.c",84);
  shmem = shmget(shmemkey, sizeof(int)*4, IPC_CREAT | 0644);
  if (shmem == -1) {printf("shmem error: %s\n",strerror(errno));}
  int *a,*b,*c,*d;
  int *e;
  a = shmat(shmem,NULL,0);
  e = a+index;
  *e = newC;
}


void setOkToStart() {
  int shmem;
  int shmemkey = ftok("cards.c",85);
  shmem = shmget(shmemkey,sizeof(int), IPC_CREAT | 0644);
  int *ok = shmat(shmem,NULL,0);
  *ok = 1;
}

int getOkToStart() {
  int shmem;
  int shmemkey = ftok("cards.c",85);
  shmem = shmget(shmemkey,sizeof(int), IPC_CREAT | 0644);
  int *ok = shmat(shmem,NULL,0);
  return *ok;
}
