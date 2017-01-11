#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "printMethods.h"


void printCard(struct card c) {
  char *values[] = {"","A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  char *suits[] = {"♦", "♣", "♥", "♠"};
  printf("Card: %s of %s\n",values[c.value],suits[c.suit]);
}


void printPlayer(struct player p) {
  char *values[] = {"","A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  char *suits[] = {"♦", "♣", "♥", "♠"};
  
  printf("Player id: %d\n",p.id);
  printf("Cards left: %d\n",p.cardsLeft);
  int i;
  printf("Hand: \n");
  for (i = 0; i < 13; i++) {
    if(p.hand[i].suit % 2 == 0){
      //Red Suits
      printf(" %s\x1B[31m %s\x1B[37m |",values[p.hand[i].value],suits[p.hand[i].suit]);
    }else{
      //Black Suits
      printf(" %s\x1B[30m %s\x1B[37m |",values[p.hand[i].value],suits[p.hand[i].suit]);
    }
  }
  printf("\n");
}
