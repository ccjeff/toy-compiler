#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "translate.h"


char* generate(char* op, int srcVal, char* dest, char* temp){
    char* outputLine = malloc(sizeof(char)* MAX_STATEMENT_LEN);
    if (strcmp(temp,"")!=0) sprintf(outputLine,"%s,%d,%s,%s",op,srcVal,dest,temp);
    else sprintf(outputLine,"%s,%d,%s",op,srcVal,dest);
    return outputLine;
}

void assignment(char* target, int param){
    char* outputLine = generate("Store",param,target,"");
    printf("in assignment translation: %s \n", outputLine);
}

void readID(char* inVar){
    char* outputLine = generate("Read",-1,inVar,"");
    printf("in read translation: %s \n", outputLine); // no real val passed yet
}

void writeID(int outVar){
    char* outputLine = generate("Write",outVar,"temp","");
    printf("in write translation: %s \n", outputLine); // no real val passed yet
}





FILE* translateRoutine(char* filename){
    FILE* outputFP = fopen(filename, "w");
    if(outputFP == NULL){
      printf("Error!");   
      exit(1);             
    }
    return outputFP;
}