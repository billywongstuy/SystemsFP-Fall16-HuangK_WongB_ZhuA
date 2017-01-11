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




int getInput(int * chosen) {
  char in[500];
  printf("Enter your move: ");
  fgets(in,sizeof(in),stdin);

  strtok(in,"\n");

  char *inS = in;
  int count = 0;
  char * t;
  int n;
  
  while (t = strsep(&inS,",")) {
    n = atoi(stripper(t));
    //IF INVAlID OPTION, MAKES THE FIRST INDEX NULL
    if (n == 0 || n > 13) {
      chosen[0] = NULL;
      return 0;
    }
    else {
      chosen[count] = n;
    }
    count++;
  }

  return count;

}
