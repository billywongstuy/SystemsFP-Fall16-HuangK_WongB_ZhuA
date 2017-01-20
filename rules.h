#ifndef RULES_H_
#define RULES_H_

#include "cards.h"
#include "players.h"


//0 is success
//other numbers for different why invalid
int no3D(int lenC, int *choice);

int validMove(int turn, int lenC, int lenL, int * choice, int * last);

int validSingle(int * choice, int * last);

int validDouble(int * choice, int * last);

int validTriple(int * choice, int * last);

int validCombo(int * choice, int *last);

int checkCombo(int * cards);

int checkStraight(int * cards);

int checkFlush(int * cards);

int checkHouse(int * cards);

int checkBomb(int * cards);

int checkSF(int * cards);

char * getInvalidMessage(int e);
//-3 START WITH 3 OF DIAMONDS
//-2 what are you throwing out man...
//-1 wrong amount of cards *except for first turn and freebies!!!
// 0 all good 
// 1 hand not strong enough
// 2 cards are not matching
//need when game ends

#endif


