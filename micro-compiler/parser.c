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
    for (idx; idx < bufferSize; idx++)
        if (strcmp(symbolTable[idx].id, sym) == 0) return idx;
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
    int idx = findIdx(val);
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
        sprintf(symbolTable[i].id, sym);        
    } else {
        symbolTable[idx].val = val;
    }
}

static int isNum(char* s){
    for (int i = 0; i < strlen(s); i++)
        if(!isdigit(s[i])) return -1;
    return atoi(s);
}

void takesInput(char* s){
    int val;
    char temp[15];
    printf("int for %s \n", s);
    fscanf(stdin, "%s", temp);
    if ((val = isNum(temp)) == -1){
        printf("should only enter integer value \n");
        exit(EXIT_FAILURE);
    }
    updateSymbolTable(s, val);
}