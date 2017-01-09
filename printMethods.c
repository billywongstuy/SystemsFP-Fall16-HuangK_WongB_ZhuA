#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "printMethods.h"


void printCard(struct card c) {
  printf("Card %d of %c\n",c.value,c.suit);
}


void printPlayer(struct player p) {
  printf("Player id: %d\n",p.id);
  printf("Cards left: %d\n",p.cardsLeft);
  int i;
  printf("Hand: \n");
  for (i = 0; i < 13; i++) {
    printf(" %d of %c |",p.hand[i].value,p.hand[i].suit);
  }
  printf("\n");
}
