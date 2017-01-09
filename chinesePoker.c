#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "cards.h"
#include "players.h"


int main() {
  struct card deck[52];
  struct player p1,p2,p3,p4;
  
  setupDeck(deck);
  distributeCards(deck,p1,p2,p3,p4);
  
  
  struct card b[13];
  int i;
  
}
