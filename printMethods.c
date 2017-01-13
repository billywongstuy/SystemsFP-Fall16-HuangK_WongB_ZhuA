#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "printMethods.h"


char *values[] = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2"};
char *suits[] = {"♦", "♣", "♥", "♠"};

void printCard(struct card c) {
  printf("Card: %s of %s\n",values[c.value],suits[c.suit]);
}


void printPlayer(struct player p) {

  printf("\x1B[1;37mPlayer id: %d\n",p.id);
  printf("Cards left: %d\n",p.cardsLeft);
  int i;
  printf("Hand: \n");
  for (i = 0; i < 13; i++) {
    /*if(p.hand[i].suit % 2 == 0){
      //Red Suits
      printf(" %s\x1B[1;31m %s\x1B[37m |",values[p.hand[i].value],suits[p.hand[i].suit]);
    }else{
      //Black Suits
      printf(" %s\033[1;34m %s\x1B[37m |",values[p.hand[i].value],suits[p.hand[i].suit]);
  }*/
    if(p.hand[i].value == 7){
        printf("%s", values[p.hand[i].value]);
    }else{
        printf(" %s", values[p.hand[i].value]);
    }
    if(p.hand[i].suit % 2 == 0){
        printf("\x1B[31m %s\x1B[37m | ", suits[p.hand[i].suit]);
    }else{
        printf("\x1B[34m %s\x1B[37m | ", suits[p.hand[i].suit]);
    }
  }
  printf("\x1B[0m\n\n");
}


char * printPlayerClient(struct player p) {
  char buf[500];
  
  printf("\x1B[1;37mPlayer id: %d\n",p.id);
  printf("Cards left: %d\n",p.cardsLeft);
  int i;
  printf("Hand: \n");
  for (i = 0; i < 13; i++) {
    printf("before ifs\n");
    if(p.hand[i].value == 7){
      printf("before if1\n");
      sprintf(buf + strlen(buf), "%s", values[p.hand[i].value]);
    }else{
      printf("before if2\n");
      sprintf(buf + strlen(buf), "%s", values[p.hand[i].value]);
      printf("after if2\n");
    }
    if(p.hand[i].suit % 2 == 0){
      printf("before if2\n");
      sprintf(buf + strlen(buf), "\x1B[31m %s\x1B[37m | ", suits[p.hand[i].suit]);
    }else{
      printf("before if3\n");
      sprintf(buf + strlen(buf), "\x1B[34m %s\x1B[37m | ", suits[p.hand[i].suit]);
    }
    printf("end-a\n");
  }
  printf("\x1B[0m\n\n");
  return buf;
}
