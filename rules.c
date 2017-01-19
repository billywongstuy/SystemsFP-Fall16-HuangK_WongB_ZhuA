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


int validMove(int lenC, int lenL, struct card * choice, struct card * last){
  if(lenC != lenL){
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

int validSingle(struct card * choice, struct card * last){
  int val1 = choice[0].value*10 + choice[0].suit;
  int val2 = last[0].value*10 + last[0].suit;

  if(val1<val2){
    return 1;
    //Not big enough
  }
  
  return 0;
}

int validDouble(struct card * choice, struct card * last){
  int doubleCount;
  int checkVal = choice[0].value;
  for(doubleCount=1; doubleCount<3; doubleCount++){
    if(choice[doubleCount].value != checkVal){
      return 2;
      //Not matching
    }
  }

  if(choice[0].value < last[0].value){
    return 1;
  }
  if(choice[0].value == last[0].value){
    if(choice[0].suit < last[0].suit){
      return 1;
    }
  }

  return 0;
}

int validTriple(struct card * choice, struct card * last){
  int tripleCount;
  int checkVal = choice[0].value;
  for(tripleCount=1; tripleCount<4; tripleCount++){
    if(choice[tripleCount].value != checkVal){
      return 2;
      //Not matching
    }
  }

  if(choice[0].value < last[0].value){
    return 1;
  }

  return 0;
}

int validCombo(struct card * cards, struct card * last){
  //int cardsCombos[5] = {1, 2, 3, 4, 5};
  //1: Straight
  //2: Flush
  //3: Full House
  //4: Bomb
  //5: Straight flush
  
  /*int cardsCount;
  int cardsVal = cards[0].value;
  for(cardsCount = 1; cardsCount<5; cardsCount++){
    if(cards[cardsCount].value != cardsVal){
      
    }
    }*/
  int cardsCount;
  int hand[5];
  for(cardsCount = 0; cardsCount<5; cardsCount++){
    hand[cardsCount] = cards[cardsCount].value*10 + cards[cardsCount].suit;
  }
  int handCombo = checkCombo(hand);

  cardsCount=0;
  int opponent[5];
  for(cardsCount = 0; cardsCount<5; cardsCount++){
    opponent[cardsCount] = cards[cardsCount].value*10 + cards[cardsCount].suit;
  }
  int opponentCombo = checkCombo(hand);

  if(handCombo < opponentCombo){
    return 1;
  }
  if(handCombo == opponentCombo){
    if(hand[2] < opponent[2]){
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
