#ifndef PLAYERS_H_
#define PLAYERS_H_

#include "cards.h"

typedef struct player {
  struct card hand[13];
  int cardsLeft;
  int id;
} player;


void initializePlayer(struct player *p);

void distributeCards(struct card * deck, struct player *p1, struct player *p2, struct player *p3, struct player *p4);

#endif
