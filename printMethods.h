#ifndef PRINT_H_
#define PRINT_H_

#include "cards.h"
#include "players.h"


void printCard(struct card c);
void printPlayer(struct player p);
char * printPlayerClient(struct player p);

#endif
