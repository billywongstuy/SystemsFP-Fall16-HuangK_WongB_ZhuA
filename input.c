#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "printMethods.h"


char* stripper(char* stripping){
    while(stripping[0] == ' '){
        *stripping++;
    }
    while(stripping[strlen(stripping)-1] == ' '){
        stripping[strlen(stripping)-1] = 0;
    }
    int counter = 0;
    int length = strlen(stripping)-1;
    for(counter; counter<length; counter++){
        if(stripping[counter] == ' ' && stripping[counter+1]==' '){
            int changer = counter;
            while(changer<length){
                stripping[changer]=stripping[changer+1];
                changer++;
            }
            counter--;
            stripping[length]=0;
            length--;
        }
    }

    char *stripped = stripping;
    //printf("stripped: %s\n", stripped);
    return stripped;
}



//get input length
int getInput(int * chosen, char * in, int len) {
  
  strtok(in,"\n");
  
  char *inS = in;
  //printf("inS: %s\n", inS);
  int count = 0;
  int replacer = 0;
  char * t;
  int n;

  while(replacer < strlen(inS)){
    if(inS[replacer]==','){
      inS[replacer]=' ';
    }
    replacer++;
  }
  inS = stripper(inS);
  //printf("New: %s\n", inS);
  
  while (t = strsep(&inS," ")) {
    n = atoi(t);
    //IF INVAlID OPTION, MAKES THE FIRST INDEX NULL
    //VALID OPTIONS ARE 1-13
    if (n <= 0 || n > len) {
      chosen[0] = 0;
      //printf("wrong index\n");
      return 0;
    }
    else {
      chosen[count] = n;
    }
    count++;
  }
  
  
  int c = count;
  for (c; c < 5; c++) {
    chosen[c] = 0;
  }
  
  //printf("count: %d\n", count);
  return count;
    
}

//Card choices will always be correct because of getInput handling
char *  getCardsChosen(struct card *a, int * choices, int len, struct player ** players, int player) {
    int i = 0;
    struct card adding;
    while (i < len) {
        //SHOULDN'T TRIGGER THIS IF STATEMENT (DEBUG)
        if (choices[i] <= 0) {
            a[i].value = -1;
            return "Invalid selection/s";
        }
        else {
            adding = players[player]->hand[choices[i]-1];
            a[i] = adding;
            i++;
        }
    }
    return "";
}
