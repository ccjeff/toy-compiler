#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

//symbol table
SYMBOL symbolTable[bufferSize];

void initSymbolTable(){
    for (int i = 0; i < bufferSize; i++){
        symbolTable[i].val = -1;
    }
}

int findIdx(char* sym){
    int idx = 0;
    for (idx; idx < bufferSize; idx++){
        if (!strcmp(symbolTable[idx].id, sym)) return idx;
    }
    return -1;
}

int symbolVal(char* sym){
    int idx = findIdx(sym);
    if (idx == -1){
        printf("cannot find symbol with assigned: %s \n", sym);
        exit(EXIT_FAILURE);
    }
    return symbolTable[idx].val;
}

void updateSymbolTable(char* sym, int val){
    int idx = findIdx(sym);
    if (idx == -1){
        int i = 0;
        for (i; i < bufferSize && symbolTable[i].val != -1; i++); // finds the first empty idx
        if (i > bufferSize - 1){
            printf("No more empty space in symbol table \n");
            // need to call resize routine
            return;
        }
        //Assign ID with new value
        symbolTable[i].val = val;
        sprintf(symbolTable[i].id, "%s", sym);
        printf("Declare,%s,Integer\n", sym);        
    } else {
        symbolTable[idx].val = val;
    }
}

static int isNum(char* s){
    for (int i = 0; i < strlen(s); i++)
        if(!isdigit(s[i])) return -1;
    return atoi(s);
}

void assignEntry(char* sym){
    int val = -2;
    // char temp[15];
    // printf("please input int for %s \n", sym);
    // fscanf(stdin, "%s", temp);
    // if ((val = isNum(temp)) == -1){
    //     printf("should only enter integer value \n");
    //     exit(EXIT_FAILURE);
    // }
    updateSymbolTable(sym, val);
}

char* get_temp(void) {	
	/* max temporary allocated so far*/
   	static int max_temp = 0;	
   	static char tempname[MAXIDLEN];
   	max_temp++;
   	sprintf(tempname, "Temp&%d", max_temp);
   	assignEntry(tempname);
   	return tempname;
}