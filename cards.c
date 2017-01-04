#include <stdio.h>
#include <stdlib.h>

#include "cards.h"


struct card deck[52];

void setupDeck(struct card * deck) {
  int i = 0;
  int v;
  int s;

  for (v = 1; v <= 13; v++) {
    for (s = 0; s < 4; s++) {
      char su = 'D';
      if (s == 1) {
	su = 'C';
      }
      if (s == 2) {
	su = 'H';
      }
      if (s == 3) {
	su = 'S';
      }
      struct card c;
      c.suit = su;
      c.value = v;
      deck[i] = c;
      i++;
    }
  }
}

int main() {
  struct card test;
  setupDeck(deck);
  
  test.value = 4;
  test.suit = 'S';

  printf("Card info: %d of %c\n",test.value,test.suit);
  for (int i = 0; i < sizeof(deck)/sizeof(deck[i]); i++) {
    printf("Card %d: %d of %c\n",i,deck[i].value,deck[i].suit);
    
  }
  
}
