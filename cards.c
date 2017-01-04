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


int suitNum(card c) {
  if (c.suit == 'D') {
    return 0;
  }
  else if (c.suit = 'C') {
    return 1;
  }
  else if (c.suit = 'H') {
    return 2;
  }
  else if (c.suit = 'S') {
    return 3;
  }
  return -1;
}


int valNum(card c) {
  if (c.value <= 2) {
    return c.value + 13;
  }
  return c.value;
}

//special cases for 2 and A
card greaterCard(struct card c1, struct card c2) {
  int c1Suit = suitNum(c1);
  int c2Suit = suitNum(c2);
  int c1Val = valNum(c1);
  int c2Val = valNum(c2);
  
  
  if (c1Val == c2Val) {
    if (c1Suit >= c2Suit) {
      return c1;
    }
    return c2;
  }
  else if (c1Val > c2Val) {
    return c1;
  }
  return c2;
}

int main() {
  struct card test;
  test.value = 4;
  test.suit = 'S';
  
  setupDeck(deck);

  struct card g = greaterCard(deck[0],deck[23]);
  
  printf("Card %d of %c\n",g.value,g.suit);
  
}
