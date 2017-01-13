#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

#include "networking.h"
#include "cards.h"
#include "players.h"
#include "printMethods.h"
#include "input.h"
#include "turns.h"
#include "rules.h"

//change setup and variables when it changes in cPoker.c
//change setup in networking.c
//change variables here, client.c, networking.c, networking.h
struct card deck[52];
struct player p1,p2,p3,p4;
struct player *players[4];
int mode;  //0 for debug, 1 for single, 2 for double, 3 for triple, 5 for 5-combo
int turnPlayer; //is an index for player who has to make their move
int turnNumber; //use this to check player turn
char * turnPlayerInfo;
int step;


void process( char * s );
void sub_server( int sd );
void step1();

int main() {

  int sd, connection;

  sd = server_setup();

  printf("here we go");
  setup();
  turnPlayerInfo = "hello";
  
  while (1) {

    printf("loop");
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
  }
  return 0;
}


void sub_server( int sd ) {

  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    printf("processed\n");
    write( sd, buffer, sizeof(buffer));    //This is what is passed to client
  }
  
}



void process( char * s ) {
  srand(time(NULL));

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

    int first = getFirstPlayer(players,4,13);
    //int first = 0;
    
    char * error = getCardsChosen(selected,chosen,len,players,first);
    sortCards(selected,len);
    
    while (count < len && selected[count].value != -1) {
      printCard(selected[count]);
      count++;
    }

    
  }
}


