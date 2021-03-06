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
  if(e == -4){
    return "\x1B[31mPlease don't repeat cards...\x1B[0m\n";
  }
  if(e == -3){
    return "\x1B[31mStart with 3 of Diamonds\x1B[0m\n";
  }
  if(e == -2){
    return "\x1B[31mNot valid hand\x1B[0m\n";
  }
  if(e == -1){
    return "\x1B[31mWrong number of cards\x1B[0m\n";
  }
  if(e == 1){
    return "\x1B[31mNot strong enough\x1B[0m\n";
  }
  if(e == 2){
    return "\x1B[31mCards don't match\x1B[0m\n";
  }
  if(e == 3){
    return "\x1B[31mCan't end with 2 of Spades\x1B[0m\n";
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
  if(lenC > 5){
    return -1;
  }

  if(lenC > 1){
    int handCheck;
    int repeating = choice[0];
    for(handCheck = 1; handCheck < lenC; handCheck++){
      if(choice[handCheck]==repeating){
        return -4;
      }
      repeating = choice[handCheck];
    }
  }

  if(turn == 1){
    if(no3D(lenC, choice)){
      return -3;
    }

    int counter;
    int setWeird=0;
    for(counter = 0; counter<5 ; counter++){
      last[counter] = setWeird;
      setWeird-=2;
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
    return validCombo(choice, last,turn);
  }


  return 0;
  //Working!
}

int validSingle(int * choice, int * last){
  if(choice[0]<last[0]){
    printf("SINGLE BAD!\n");
    return 1;
    //Not big enough
  }
  printf("SINGLE GOOD!\n");

  return 0;
}

int validDouble(int * choice, int * last){
  int doubleCount;
  int checkVal = choice[0]/10;
  for(doubleCount=1; doubleCount<2; doubleCount++){
    if(choice[doubleCount]/10 != checkVal){
      printf("DOUBLE BAD!\n");
      return 2;
      //Not matching
    }
  }

  if(choice[0]/10 < last[0]/10){
    printf("DOUBLE BAD!\n");
    return 1;
  }
  if(choice[0]/10 == last[0]/10){
    if(choice[0]%10 < last[0]%10){
      printf("DOUBLE BAD!\n");
      return 1;
    }
  }

  printf("DOUBLE GOOD!\n");
  return 0;
}

int validTriple(int * choice, int * last){
  int tripleCount;
  int checkVal = choice[0]/10;
  for(tripleCount=1; tripleCount<3; tripleCount++){
    if(choice[tripleCount]/10 != checkVal){
      printf("TRIPLE BAD!\n");
      return 2;
      //Not matching
    }
  }

  if(choice[0]/10 < last[0]/10){
    printf("TRIPLE BAD!\n");
    return 1;
  }

  printf("TRIPLE GOOD!\n");
  return 0;
}

int validCombo(int * cards, int * last, int turn){
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
  if(handCombo == -2){
    printf("COMBO BAD!\n");
    return -2;
  }

  /*cardsCount=0;
  int opponent[5];
  for(cardsCount = 0; cardsCount<5; cardsCount++){
    opponent[cardsCount] = cards[cardsCount]/10 + cards[cardsCount]%10;
    }*/

  if (turn == 1) {
    return 0;
  }

  int opponentCombo = checkCombo(last);

  if(handCombo < opponentCombo){
    printf("COMBO BAD!\n");
    return 1;
  }
  if(handCombo == opponentCombo){
    if(handCombo == 1 || handCombo == 2 || handCombo == 5){
      if(cards[0] < last[0]){
	printf("Straight, Flush, SF BAD!\n");
	return 1;
      }
    }else{
      if(cards[2] < last[2]){
	printf("House, Bomb BAD!\n");
	return 1;
      }
    }
  }

  printf("COMBO BAD!\n");
  return 0;
}

int checkCombo(int *cards){
  if(!checkSF(cards)){
    printf("SF!\n");
    return 5;
  }else if(!checkBomb(cards)){
    printf("Bomb!\n");
    return 4;
  }else if(!checkHouse(cards)){
    printf("House!\n");
    return 3;
  }else if(!checkFlush(cards)){
    printf("Flush!\n");
    return 2;
  }else if(!checkStraight(cards)){
    printf("Straight!\n");
    return 1;
  }

  printf("COMBO BAD!\n");
  return -2;
  //Not a thing;
}

int checkStraight(int * cards){
  int value = cards[0]/10;
  int counter;
  for(counter = 1; counter<5; counter++){
    value--;
    if(cards[counter]/10 != value){
      printf("STRAIGHT BAD!\n");
      return 3;
      //not valid card
    }
  }

  printf("STRAIGHT GOOD!\n");
  return 0;
}

int checkFlush(int *cards){
  int suit = cards[0]%10;
  int counter;
  for(counter = 1; counter<5; counter++){
    if(cards[counter]%10 != suit){
      printf("FLUSH BAD!\n");
      return 3;
    }
  }

  printf("FLUSH GOOD!\n");
  return 0;
}

int checkHouse(int *cards){
  int value = cards[2]/10;

  if(cards[0]/10 == value){
    if(cards[1]/10 != value){
      printf("HOUSE BAD!\n");
      return 3;
    }
    if(cards[3]/10 != cards[4]/10){
      printf("HOUSE BAD!\n");
      return 3;
    }
  }else if(cards[4]/10 == value){
    if(cards[3]/10 != value){
      printf("HOUSE BAD!\n");
      return 3;
    }
    if(cards[0]/10 != cards[1]/10){
      printf("HOUSE BAD!\n");
      return 3;
    }
  }else{
    return -2;
  }

  printf("HOUSE GOOD!\n");
  return 0;
}

int checkBomb(int *cards){
  int value = cards[1]/10;
  int counter;
  if(cards[0]/10 == value){
    for(counter=2; counter<4; counter++){
      if(cards[counter]/10 != value){
	printf("BOMB BAD!\n");
	return 3;
      }
    }
  }else{
    for(counter=2; counter<5; counter++){
      if(cards[counter]/10 != value){
	printf("BOMB BAD!\n");
	return 3;
      }
    }
  }

  printf("BOMB GOOD!\n");
  return 0;
}

int checkSF(int *cards){
  if(checkStraight(cards) || checkFlush(cards)){
    printf("SF BAD!\n");
    return 3;
  }

  printf("SF GOOD!\n");
  return 0;
}
