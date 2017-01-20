#ifndef NETWORKING_H
#define NETWORKING_H

#define MESSAGE_BUFFER_SIZE 1000

#include "cards.h"
#include "players.h"

void error_check( int i, char *s );

int server_setup();
int server_connect(int sd);

int client_connect( char *host );

void setup();


int getTurnPlayer();
void setTurnPlayer(int n);

char * getLastMove();
void setLastMove(char * string);

int * getAllCardsLeft();
void setAllCardsLeft(int newC, int index);

void setLastAmount(int n);
int getLastAmount();

void setLastCards(int * ar, int len);
int * getLastCards();

void setUsedAmount(int n);
int getUsedAmount();

void setUsedCards(int * ar, int len);
int * getUsedCards();


#endif
