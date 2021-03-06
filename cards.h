#ifndef CARDS_H_
#define CARDS_H_

typedef struct card {
  int value;  // 11 is Jack, 12 is Queen, 13 is King, 1 is A
  int suit;  //chars is better: D, C, H, S no need to strcmp, == is fine
} card;


//special cases are A and 2
int compareCards(struct card c1, struct card c2);

void setupDeck(struct card * deck);

#endif
