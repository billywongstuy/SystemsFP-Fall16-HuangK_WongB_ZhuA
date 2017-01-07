#ifndef PLAYERS_H_
#define PLAYERS_H_

#include "cards.h"

typedef struct player {
  struct card hand[13];
  int cardsLeft;
  int id;
} player;


#endif
