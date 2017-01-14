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

struct card * getDeck();
struct player getp1();
struct player getp2();
struct player getp3();
struct player getp4();
struct player ** getPlayers();
int getMode();
int getTurnPlayer();
int getTurnNumber();
char * getTurnPlayerInfo();
int getStep();


void setTurnPlayer(int n);

#endif
