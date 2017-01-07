#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "cards.h"
#include "players.h"


int main() {
  player p1;
  struct card b[13];
  int i;
  for (i = 0; i < 13; i++) {
    p1.hand[i].suit = 'D';
    p1.hand[i].value = i+1;
  }
  p1.cardsLeft = 13;
  p1.id = 0;
  for (i = 0; i < 13; i++) {
    printf("%d of %c\n",p1.hand[i].value,p1.hand[i].suit);
  }
}
