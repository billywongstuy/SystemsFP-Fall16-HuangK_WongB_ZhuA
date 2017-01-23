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
#include "turns.h"
#include "rules.h"

char * getInvalidMessage(int e){
  if(!e){
    return "";
  }
  if(e == -3){
    return "Start with 3 of Diamonds";
  }
  if(e == -2){
    return "Not valid hand";
  }
  if(e == -1){
    return "wrong number of cards";
  }
  if(e == 1){
    return "Not strong enough";
  }
  if(e == 2){
    return "Cards don't match";
  }
  if(e == 3){
    return "Can't end with 2 of Spades";
  }
  
}

int no3D(int lenC, int * choice){
  if(choice[lenC-1] != 0){
    printf("first move check/n");
    return 1;
    //Start with 3 of Diamonds!
  }
  return 0;
}

int validMove(int turn, int lenC, int lenL, int * choice, int * last){
  if(turn == 1){
    if(no3D(lenC, choice)){
      return -3;
    }
    
    int counter;
    for(counter = 0; counter<lenL ; counter++){
      last[counter] = 0;
    }
  } else if(lenC != lenL){
    return -1;
    //Wrong amount of cards
  }
  
  if(lenC == 1){
    return validSingle(choice, last);
  }else if(lenC == 2){
    return validDouble(choice, last);
  }else if(lenC == 3){
    return validTriple(choice, last);
  }else if(lenC == 5){
    return validCombo(choice, last);
  }
  
  
  return 0;
  //Working!
}

int validSingle(int * choice, int * last){
  if(choice[0]<last[0]){
    return 1;
    //Not big enough
  }
  
  return 0;
}

int validDouble(int * choice, int * last){
  int doubleCount;
  int checkVal = choice[0]/10;
  for(doubleCount=1; doubleCount<2; doubleCount++){
    if(choice[doubleCount]/10 != checkVal){
      return 2;
      //Not matching
    }
  }

  if(choice[0]/10 < last[0]/10){
    return 1;
  }
  if(choice[0]/10 == last[0]/10){
    if(choice[0]%10 < last[0]%10){
      return 1;
    }
  }

  return 0;
}

int validTriple(int * choice, int * last){
  int tripleCount;
  int checkVal = choice[0]/10;
  for(tripleCount=1; tripleCount<3; tripleCount++){
    if(choice[tripleCount]/10 != checkVal){
      return 2;
      //Not matching
    }
  }

  if(choice[0]/10 < last[0]/10){
    return 1;
  }

  return 0;
}

int validCombo(int * cards, int * last){
  //int cardsCombos[5] = {1, 2, 3, 4, 5};
  //1: Straight
  //2: Flush
  //3: Full House
  //4: Bomb
  //5: Straight flush
  
  /*int cardsCount;
  int cardsVal = cards[0]/10;
  for(cardsCount = 1; cardsCount<5; cardsCount++){
    if(cards[cardsCount]/10 != cardsVal){
      
    }
    }*/
  /*int cardsCount;
  int hand[5];
  for(cardsCount = 0; cardsCount<5; cardsCount++){
    hand[cardsCount] = cards[cardsCount]/10 + cards[cardsCount]%10;
    }*/
  int handCombo = checkCombo(cards);

  /*cardsCount=0;
  int opponent[5];
  for(cardsCount = 0; cardsCount<5; cardsCount++){
    opponent[cardsCount] = cards[cardsCount]/10 + cards[cardsCount]%10;
    }*/
  int opponentCombo = checkCombo(last);

  if(handCombo < opponentCombo){
    return 1;
  }
  if(handCombo == opponentCombo){
    if(cards[2] < last[2]){
      return 1;
    }
  }
  
  return 0;
}

int checkCombo(int *cards){
  if(!checkSF(cards)){
    return 5;
  }else if(!checkBomb(cards)){
    return 4;
  }else if(!checkHouse(cards)){
    return 3;
  }else if(!checkFlush(cards)){
    return 2;
  }else if(!checkStraight(cards)){
    return 1;
  }

  return -2;
  //Not a thing;
}

int checkStraight(int * cards){
  int value = cards[0]/10;
  int counter;
  for(counter = 1; counter<5; counter++){
    if(cards[counter]/10 != value--){
      return 3;
      //not valid card
    }
    value--;
  }

  return 0;
}

int checkFlush(int *cards){
  int suit = cards[0]%10;
  int counter;
  for(counter = 1; counter<5; counter++){
    if(cards[counter]%10 != suit){
      return 3;
    }
  }

  return 0;
}

int checkHouse(int *cards){
  int value = cards[2]/10;

  if(cards[0]/10 == value){
    if(cards[1]/10 != value){
      return 3;
    }
    if(cards[3]/10 != cards[4]/10){
      return 3;
    }
  }
  if(cards[4]/10 == value){
    if(cards[3]/10 != value){
      return 3;
    }
    if(cards[0]/10 != cards[1]/10){
      return 3;
    }
  }

  return 0;
}

int checkBomb(int *cards){
  int value = cards[1]/10;
  int counter;
  if(cards[0]/10 == value){
    for(counter=2; counter<4; counter++){
      if(cards[counter]/10 != value){
	return 3;
      }
    }
  }else{
    for(counter=2; counter<5; counter++){
      if(cards[counter]/10 != value){
	return 3;
      }
    }
  }

  return 0;
}

int checkSF(int *cards){
  if(checkStraight(cards) || checkFlush(cards)){
    return 3;
  }

  return 0;
}
