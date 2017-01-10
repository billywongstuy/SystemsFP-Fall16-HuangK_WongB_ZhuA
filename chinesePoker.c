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

  //  \e[37m\e[46m
  printf("\033[2J\033[;H");
  printf("\x1B[37m");

  
  struct card deck[52];
  struct player p1,p2,p3,p4;
  struct player *players[4];

  players[0] = &p1;
  players[1] = &p2;
  players[2] = &p3;
  players[3] = &p4;

  setupDeck(deck);
  distributeCards(deck,players,4);
  
  sortHand(p1.hand);
  sortHand(p2.hand);
  sortHand(p3.hand);
  sortHand(p4.hand);
  
  printPlayer(p1);
  printPlayer(p2);
  printPlayer(p3);
  printPlayer(p4);

  
}
