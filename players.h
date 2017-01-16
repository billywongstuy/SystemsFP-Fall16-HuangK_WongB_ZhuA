#ifndef PLAYERS_H_
#define PLAYERS_H_

#include "cards.h"

typedef struct player {
  struct card hand[13];
  int cardsLeft;
  int id;
} player;

//deprecated
void initializePlayer(struct player *p);

void initializePlayers(struct player **p, int numP);

void distributeCards(struct card * deck, struct player ** players, int numP);

void sortCards(struct card *hand, int len);

void useCards(struct player *p, int *indices, int indLen);

#endif
