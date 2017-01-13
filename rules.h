#ifndef RULES_H_
#define RULES_H_

#include "cards.h"
#include "players.h"


//1 is success
//other numbers for different why invalid
int validMove(struct card * choice);

char * getInvalidMessage(int e);



#endif
