#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#include "cards.h"
#include "players.h"
#include "printMethods.h"
#include "input.h"

int main() {
  srand(time(NULL));

  //  \e[37m\e[46m
  //printf("\033[2J\033[;H");
  //printf("\x1B[46m\x1B[37m");


  struct card deck[52];
  struct player p1,p2,p3,p4;
  struct player *players[4];

  players[0] = &p1;
  players[1] = &p2;
  players[2] = &p3;
  players[3] = &p4;

  setupDeck(deck);
  distributeCards(deck,players,4);

  sortCards(p1.hand,13);
  sortCards(p2.hand,13);
  sortCards(p3.hand,13);
  sortCards(p4.hand,13);

  printPlayer(p1);
  printPlayer(p2);
  printPlayer(p3);
  printPlayer(p4);


  /*
  char in[500];
  printf("Enter your move: ");
  fgets(in,sizeof(in),stdin);

  strtok(in,"\n");
  printf("In: %s\n",in);

  char * chosen[5];
  char *ina = in;
  int count = 0;
  while (chosen[count] = (strsep(&ina,","))) {
    chosen[count] = stripper(chosen[count]);
    count++;
  }

  count = 0;
  while (chosen[count] && count < 5) {
    printf("c: %s\n",chosen[count]);
    count++;
  }
  */


  int chosen[5];
  int len = getInput(chosen);


  int count = 0;
  while (chosen[count] && count < len) {
    printf("c: %d\n",chosen[count]);
    count++;
  }


  //get the specific cards and sort them
  struct card selected[len];
  count = 0;
  while (count < len) {
    selected[count] = p1.hand[chosen[count]-1];
    count++;
  }

  sortCards(selected,len);

  count = 0;
  while (count < len) {
    printCard(p1.hand[count]);
    count++;
  }




  //atoi returns 0 if no int found

}
