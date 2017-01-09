#include <stdio.h>
#include <stdlib.h>

#include "players.h"


void initializePlayers(struct player **p, int numP) {
  int i;

  for (i = 0; i < numP; i++) {
    p[i]->cardsLeft = 13;
    p[i]->id = i;
  }
}


void cardSwap(struct card *deck, int in1, int in2) {
  struct card temp = deck[in1];
  deck[in1] = deck[in2];
  deck[in2] = temp;
}

void distributeCards(struct card * deck, struct player ** players, int numP) {
  int deckCardsLeft = 52;
  int i,j;
  int random;


  initializePlayers(players,numP);
  
  for (i = 0; i < numP; i++) {

    for (j = 0; j < 13; j++) {
      
      random = rand()%deckCardsLeft;
      
      players[i]->hand[j] = deck[random];
      cardSwap(deck,random,deckCardsLeft-1);
      deckCardsLeft--;
      
    }
  }
      
    
}

