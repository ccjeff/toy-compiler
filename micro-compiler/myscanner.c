#include <stdio.h>
#include "scanner.h"

extern int yylex();
extern int yylineno;
extern char* yytext;

int main(void){
	int ntoken = yylex();
	while(ntoken) {
		printf("%d\n", ntoken);
		ntoken = yylex();
	}
	return 0;
}
