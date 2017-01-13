#ifndef INPUT_H_
#define INPUT_H_

//char * getInput();

#include "players.h"
#include "cards.h"

int getInput(int * chosen);

char* stripper(char* stripping);

char * getCardsChosen(struct card *a, int * choices, int len, struct player ** players, int player);


#endif
