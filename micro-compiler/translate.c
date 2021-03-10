#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "translate.h"
#include "parser.h"


char* generate(char* op, expr_rec source, expr_rec dest, char* temp){
    char* outputLine = malloc(sizeof(char)* MAX_STATEMENT_LEN);
    switch(dest.kind){
        case IDEXPR:
            // assign is of for ID := ID
            if (strcmp(temp,"") == 0) sprintf(outputLine, "%s,%s,%s", op, source.name, dest.name);
            else {
                if (source.kind == LITERALEXPR) sprintf(outputLine, "%s,%s,%d,%s", op, temp, source.val, dest.name);
                else sprintf(outputLine, "%s,%s,%s,%s", op, temp, source.name, dest.name);
            }
            break;
        case LITERALEXPR:
            // assign ID := 1
            if (strcmp(temp,"") == 0) sprintf(outputLine, "%s,%s,%d", op, source.name, dest.val);
            else {
                if (source.kind == LITERALEXPR) sprintf(outputLine, "%s,%s,%d,%d", op, temp, source.val, dest.val);
                else sprintf(outputLine, "%s,%s,%s,%d", op, temp, source.name, dest.val);
            }
            
            break;
        case TEMPEXPR:
            // ID := a + 12 + c
            if (strcmp(temp,"") == 0) sprintf(outputLine, "%s,%s,%s", op, source.name, dest.name);
            else {
                if (source.kind == LITERALEXPR) sprintf(outputLine, "%s,%d,%s,%s", op, source.val, dest.name, temp);
                else sprintf(outputLine, "%s,%s,%s,%s", op, source.name, dest.name, temp);
            }
            
            break;
    }
    return outputLine;
}

expr_rec gen_infix(expr_rec e1, char* op, expr_rec e2){
	/* Generate code for infix operation.
	 * Get result temp and set up semantic 
	 * record for result. 	 */
    char* outputLine = malloc(sizeof(char)* MAX_STATEMENT_LEN);
    expr_rec e_rec;
	/* An expr_rec with temp variant set. */
	e_rec.kind = TEMPEXPR;
	strcpy(e_rec.name, get_temp());
    outputLine = generate(op, e1, e2, e_rec.name);
    printf("%s\n",outputLine);
	return e_rec;
}


void assignment(expr_rec target, expr_rec source){
    char* outputLine = generate("Store",target,source,"");
    printf("%s \n", outputLine);
}

void readID(expr_rec inVar){
    printf("Read,%s\n", inVar.name); // no real val passed yet
}

void writeID(expr_rec outVar){
    printf("Write,%s\n", outVar.name);
}

void finalize(){
    printf("Halt\n");
}