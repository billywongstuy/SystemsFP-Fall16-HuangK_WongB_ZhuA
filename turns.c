#include <stdio.h>
#include <stdlib.h>

#include "cards.h"
#include "players.h"
#include "turns.h"

int nextPlayer(int numP, int currP) {
  printf("index of current player: %d\n",currP);
  printf("length: %d\n",numP);
  if (currP == numP - 1) {
    return 0;
  }
  return currP + 1;
}

int getFirstPlayer(struct player ** players, int numP, int handSize) {
  int i;
  struct card check;
  for (i = 0; i < numP; i++) {
    check = players[i]->hand[handSize-1];
    if (check.suit == 0 && check.value == 0) {
      return i;
    }
  }
  return -1;
}

//no players = 4
//current players = 3
