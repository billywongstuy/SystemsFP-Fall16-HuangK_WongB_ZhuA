#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "printMethods.h"


char *values[] = {" 3", " 4", " 5", " 6", " 7", " 8", " 9", "10", " J", " Q", " K", " A", " 2"};
char *suits[] = {"♦", "♣", "♥", "♠"};

void printCard(struct card c) {
  printf("Card: %s of %s\n",values[c.value],suits[c.suit]);
}


void printPlayer(struct player p) {

  printf("\x1B[1;37mPlayer id: %d\n",p.id);
  printf("Cards left: %d\n",p.cardsLeft);
  int i;
  printf("Hand: \n");
  for (i = 0; i < p.cardsLeft; i++) {
    /*if(p.hand[i].suit % 2 == 0){
      //Red Suits
      printf(" %s\x1B[1;31m %s\x1B[37m |",values[p.hand[i].value],suits[p.hand[i].suit]);
    }else{
      //Black Suits
      printf(" %s\033[1;34m %s\x1B[37m |",values[p.hand[i].value],suits[p.hand[i].suit]);
  }*/
    //if(p.hand[i].value == 7){
        printf(" %s", values[p.hand[i].value]);
	/*}else{
        printf(" %s", values[p.hand[i].value]);
	}*/
    if(p.hand[i].suit % 2 == 0){
        printf("\x1B[31m %s\x1B[37m |", suits[p.hand[i].suit]);
    }else{
        printf("\x1B[34m %s\x1B[37m |", suits[p.hand[i].suit]);
    }
  }
  printf("\x1B[0m\n");
}


void memPrintPlayer (struct player *p) {
  printf("\x1B[1;37mPlayer id: %d\n",p->id);
  printf("Cards left: %d\n",p->cardsLeft);
  int i;
  printf("Hand: \n");
  for (i = 0; i < p->cardsLeft; i++) {
    // if(p->hand[i].value == 7){
        printf("%s", values[p->hand[i].value]);
	/*}else{
        printf(" %s", values[p->hand[i].value]);
	}*/
    if(p->hand[i].suit % 2 == 0){
        printf("\x1B[31m %s\x1B[37m |", suits[p->hand[i].suit]);
    }else{
        printf("\x1B[34m %s\x1B[37m |", suits[p->hand[i].suit]);
    }
  }
  printf("\x1B[0m\n");
}


char * printPlayerClient(struct player p) {
  char buf[1000];

  sprintf(buf, "\x1B[1;37mCards left: %d\n",p.cardsLeft);
  int i;
  sprintf(buf + strlen(buf), "Hand: \n");
  for (i = 0; i < p.cardsLeft; i++) {
    //if(p.hand[i].value == 7){
      sprintf(buf + strlen(buf), "%s", values[p.hand[i].value]);
      //}
    if(p.hand[i].suit % 2 == 0){
      sprintf(buf + strlen(buf), "\x1B[31m %s\x1B[37m |", suits[p.hand[i].suit]);
    }else{
      sprintf(buf + strlen(buf), "\x1B[34m %s\x1B[37m |", suits[p.hand[i].suit]);
    }
  }
  sprintf(buf + strlen(buf), "\x1B[0m\n");
  char * s = (char *)malloc(sizeof(char));
  strcpy(s,buf);
  return s;
}


//THE ONE SHOWN TO CLIENT
char * memPrintPlayerClient(struct player * p) {
  char buf[1000];
  int len = 0;

  len += sprintf(buf + len, "\x1B[1;37m\nYour Info (Player %d)\n\nCards left: %d\n\n",p->id+1,p->cardsLeft);
  int i;
  sprintf(buf + strlen(buf), "Hand: \n");
  for (i = 0; i < p->cardsLeft; i++) {
    len += sprintf(buf + len, "%s", values[p->hand[i].value]);
    if(p->hand[i].suit % 2 == 0){
      len += sprintf(buf + len, "\x1B[31m %s\x1B[37m |", suits[p->hand[i].suit]);
    }else{
      len += sprintf(buf + len, "\x1B[34m %s\x1B[37m |", suits[p->hand[i].suit]);
    }
  }
  len += sprintf(buf + len, "\x1B[0m\n");

  char * s = (char *)malloc(sizeof(char));
  strcpy(s,buf);
  return s;
}


char * printChoice (struct card *ch, int l, int tp) {
  char buf[1000];
  int len = 0;

  int i;
  for (i = 0; i < l; i++) {
    len += sprintf(buf + len, "\x1B[1;37m%s", values[ch[i].value]);
    if(ch[i].suit % 2 == 0){
      len += sprintf(buf + len, "\x1B[31m %s\x1B[37m |", suits[ch[i].suit]);
    }else{
      len += sprintf(buf + len, "\x1B[34m %s\x1B[37m |", suits[ch[i].suit]);
    }
  }
  len += sprintf(buf + len, " (Player %d)\x1B[0m\n\n",tp+1);

  char * s = (char *)malloc(sizeof(char));
  strcpy(s,buf);
  return s;
}
