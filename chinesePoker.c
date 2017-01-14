#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#include "cards.h"
#include "players.h"
#include "printMethods.h"
#include "input.h"
#include "turns.h"
#include "rules.h"


struct card deck[52];
struct player p1,p2,p3,p4;
//struct player players[4];
struct player *playersM[4];
int mode;  //0 for debug, 1 for single, 2 for double, 3 for triple, 5 for 5-combo
int turnPlayer; //is an index for player who has to make their move
int turnNumber = 0; //use this to check player turn


void setup() {
  /*
  players[0] = p1;
  players[1] = p2;
  players[2] = p3;
  players[3] = p4;
  
  playersM = players;
  */

  playersM[0] = &p1;
  playersM[1] = &p2;
  playersM[2] = &p3;
  playersM[3] = &p4;
  
  printf("lolol\n");
  
  setupDeck(deck);
  
  distributeCards(deck,playersM,4);

  printf("11111\n");
  
  sortCards(p1.hand,13);
  sortCards(p2.hand,13);
  sortCards(p3.hand,13);
  sortCards(p4.hand,13);
}

int main() {
  srand(time(NULL));

  printf("start\n");
  
  setup();

  printf("222\n");

  printPlayer(p1);
  printPlayer(p2);
  printPlayer(p3);
  printPlayer(p4);
  
  int count = 0;

  int chosen[5];


  char in[500];
  printf("Choose your card(s): ");
  fgets(in,sizeof(in),stdin);
  
  int len = getInput(chosen,in);

  
  if (len == 0) {
    printf("Invalid selection(s)\n");
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

    printPlayer(p1);
    memPrintPlayer(playersM[0]);

  }
}

//use of players
//getFirstPlayer
//getCardsChosen
//setup (also in server initialize)
//distributeCards


