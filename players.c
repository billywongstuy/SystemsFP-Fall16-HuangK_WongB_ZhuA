#include <stdio.h>
#include <stdlib.h>

#include "players.h"


void initializePlayer(struct player *p) {
  p->cardsLeft = 13;

  //getting specific ids is later
  p->id = 0;
}



void cardSwap(struct card *deck, int in1, int in2) {
  struct card temp = deck[in1];
  deck[in1] = deck[in2];
  deck[in2] = temp;
}

void distributeCards(struct card * deck, struct player *p1, struct player *p2, struct player *p3, struct player *p4) {
  int deckCardsLeft = 52;
  int i;
  int random;

  initializePlayer(p1);
  initializePlayer(p2);
  initializePlayer(p3);
  initializePlayer(p4);
  
  for (i = 0; i < 13; i++) {
    random = rand()%deckCardsLeft;   
    p1->hand[i] = deck[random];
    cardSwap(deck,random,deckCardsLeft-1);
    deckCardsLeft--;

    random = rand()%deckCardsLeft;   
    p2->hand[i] = deck[random];
    cardSwap(deck,random,deckCardsLeft-1);
    deckCardsLeft--;

    random = rand()%deckCardsLeft;   
    p3->hand[i] = deck[random];
    cardSwap(deck,random,deckCardsLeft-1);
    deckCardsLeft--;


    random = rand()%deckCardsLeft;   
    p4->hand[i] = deck[random];
    cardSwap(deck,random,deckCardsLeft-1);
    deckCardsLeft--;
    
    
  }

}
