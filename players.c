#include <stdio.h>
#include <stdlib.h>

#include "players.h"
#include "printMethods.h"

void initializePlayers(struct player **p, int numP) {
  int i;

  for (i = 0; i < numP; i++) {
    p[i]->cardsLeft = 13;
    p[i]->id = i;
  }
}


void cardSwap(struct card *deck, int in1, int in2) {
  struct card temp = deck[in1];
  deck[in1] = deck[in2];
  deck[in2] = temp;
}

void distributeCards(struct card * deck, struct player ** players, int numP) {
  int deckCardsLeft = 52;
  int i,j,random;

  initializePlayers(players,numP);
  
  for (i = 0; i < numP; i++) {
    for (j = 0; j < 13; j++) {
      random = rand()%deckCardsLeft;
      players[i]->hand[j] = deck[random];
      cardSwap(deck,random,deckCardsLeft-1);
      deckCardsLeft--;     
    }
  }
          
}


void sortCards(struct card *hand, int len) {
  //switched to insertion sort for effiency
  
  int i,k;
  struct card val;
  for (i = 1; i < len; i++) {
    val = hand[i];
    for (k = i - 1; k >= 0 && compareCards(hand[k],val) < 0; k--) {
      hand[k+1] = hand[k];
    }
    hand[k+1] = val;
  }
  
}




//reverse sort
void insertionSort(int *indices, int len) {
  //switched to insertion sort for effiency
  
  int i,k;
  int val;
  for (i = 1; i < len; i++) {
    val = indices[i];
    for (k = i - 1; k >= 0 && indices[k] < val; k--) {
      indices[k+1] = indices[k];
    }
    indices[k+1] = val;
  }
  
}



void useCards(struct player *p, int *indices, int indLen) {
  int i;
  insertionSort(indices,indLen);
  for (i = 0; i < indLen; i++) {
    cardSwap(p->hand,indices[i]-1,(p->cardsLeft)-1);
    p->cardsLeft--;
  }
  sortCards(p->hand,p->cardsLeft);
}


//void cardSwap(struct card *deck, int in1, int in2);
