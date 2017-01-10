#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"



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
    return stripped;
}


//not working
/*
char * getInput() {
  char in[500];
  printf("Enter your move: ");
  fgets(in,sizeof(in),stdin);

  

  //atoi returns if none found
  return in;
  }*/


/*void getInput(int * chosen) {
  char in[500];
  printf("Enter your move: ");
  fgets(in,sizeof(in),stdin);

  strtok(in,"\n");
  char * inB = in;
  int count = 0;
  int val;

  while (chosen[count] = atoi(stripper(strsep(&inB,",")))) {
    count++;
  }
  
}
*/

int getInput(int * chosen) {
  char in[500];
  printf("Enter your move: ");
  fgets(in,sizeof(in),stdin);

  strtok(in,"\n");

  char *ina = in;
  int count = 0;
  char * t;
  
  while (t = strsep(&ina,",")) {
    chosen[count] = atoi(stripper(t));
    count++;
  }

  return count;

}
