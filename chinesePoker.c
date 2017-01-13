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

int main() {
  srand(time(NULL));

  struct card deck[52];
  struct player p1,p2,p3,p4;
  struct player *players[4];
  int mode;  //0 for debug, 1 for single, 2 for double, 3 for triple, 5 for 5-combo
  int turnPlayer; //is an index for player who has to make their move
  int turnNumber = 0; //use this to check player turn

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

  printPlayer(p1);
  printPlayer(p2);
  printPlayer(p3);
  printPlayer(p4);
  
  int count = 0;

  int chosen[5];
  int len = getInput(chosen);

  
  if (len == 0) {
    printf("Invalid selection(s)\n");
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
