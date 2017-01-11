#include <stdio.h>
#include <stdlib.h>

#include "cards.h"


struct card deck[52];

void setupDeck(struct card * deck) {
  int i = 0;
  int v;
  int s;

  for (v = 0; v < 13; v++) {
    for (s = 0; s < 4; s++) {
      struct card c;
      c.suit = s;
      c.value = v;
      deck[i] = c;
      i++;
    }
  }
}


/*int suitNum(card c) {
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
  }*/


int valNum(card c) {
  if (c.value <= 2) {
    return c.value + 13;
  }
  return c.value;
}

//special cases for 2 and A
int compareCards(struct card c1, struct card c2) {
  /*int c1Suit = c1.suit;
  int c2Suit = c2.suit;
  int c1Val = valNum(c1);
  int c2Val = valNum(c2);


  if (c1Val == c2Val) {
    if (c1Suit >= c2Suit) {
      //return c1;
      return 1;
    }
    //return c2;
    return -1;
  }
  else if (c1Val > c2Val) {
    //return c1;
    return 1;
  }
  //case of c2Val > c1Val
  return -1;*/
  /*int c1Val = c1.value;
  int c2Val = c2.value;
  int c1Suit = c1.suit;
  int c2Suit = c2.suit;*/

  int c1Val = c1.value*10 + c1.suit;
  int c2Val = c2.value*10 + c2.suit;
  if(c1Val > c2Val){
      return c1Val;
  }else{
      return c2Val;
  }
}


int mainCard() {
  struct card test;
  test.value = 4;
  test.suit = 3;

  setupDeck(deck);

  //struct card g = greaterCard(deck[0],deck[23]);

  //printf("Card %d of %c\n",g.value,g.suit);

}
