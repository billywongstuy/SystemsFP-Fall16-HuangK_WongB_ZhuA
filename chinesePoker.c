#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "cards.h"
#include "players.h"
#include "printMethods.h"

int main() {
  srand(time(NULL));
  
  struct card deck[52];
  struct player p1,p2,p3,p4;
  struct player *players[4];

  players[0] = &p1;
  players[1] = &p2;
  players[2] = &p3;
  players[3] = &p4;

  
  
  setupDeck(deck);
  distributeCards(deck,players,4);
  
  
  printPlayer(p1);
  printPlayer(p2);
  printPlayer(p3);
  printPlayer(p4);
  
}
