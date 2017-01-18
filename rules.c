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

int validCombo(struct card * choice, struct card * last){
  int comboCount;
  int checkVal;
  return 0;
}
