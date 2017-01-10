#include <stdio.h>
#include <stdlib.h>

#include "players.h"


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


void sortHand(struct card *hand) {
  int len = 13;
  int i;
  struct card tempC;
  
  while (len > 0) {
    for (i = 0; i < len-1; i++) {
      //use i+1,i  because WE WANT A REVERSE SORT
      if (compareCards(hand[i+1],hand[i]) >= 1) {
	tempC = hand[i];
	hand[i] = hand[i+1];
	hand[i+1] = tempC;
      }
    }
    len--;
  }

  
}


/*
public static void bubbleSort(int[]data) {
	int end = data.length;
	while (end > 0) {
	    for (int i = 0; i < end-1; i++) {
		int toSwap;
		if (data[i+1] < data[i]) {
		    toSwap = data[i];
		    data[i] = data[i+1];
		    data[i+1] = toSwap;
		}
	    }
	    end--;
	}
	}*/
