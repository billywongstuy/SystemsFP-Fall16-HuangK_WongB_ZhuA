#ifndef RULES_H_
#define RULES_H_

#include "cards.h"
#include "players.h"


//0 is success
//other numbers for different why invalid
int validMove(int lenC, int lenL, struct card * choice, struct card * last);

int validSingle(struct card * choice, struct card * last);

int validDouble(struct card * choice, struct card * last);

int validTriple(struct card * choice, struct card * last);

int validCombo(struct card * choice, struct card *last);

int checkCombo(int * cards);

int checkStraight(int * cards);

int checkFlush(int * cards);

int checkHouse(int * cards);

int checkBomb(int * cards);

int checkSF(int * cards);

char * getInvalidMessage(int e);
//-2 what are you throwing out man...
//-1 wrong amount of cards *except for first turn and freebies!!!
// 0 all good 
// 1 hand not strong enough
// 2 cards are not matching
//need who goes first and when game ends

#endif


