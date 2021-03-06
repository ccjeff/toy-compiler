%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
// #include "scanner.h"
#include "parser.h"

void yyerror(char* s);
extern int yynerrs;
extern int yylexerrs;
extern FILE* yyin;

int yylex();
%}

%union {int num; char* id;}         /* Yacc definitions for micro lang*/
// %start line

%token BEGINN
%token END
%token READ
%token WRITE
%token LPAREN
%token RPAREN
%token SEMICOLON
// %token COMMA
%token ASSIGNOP
// %token PLUOP
// %token MINUSOP
%token <num> INTLITERAL
%token <id> ID
%token SCANEOF

%left PLUOP MINUSOP COMMA
%type <num> exp primary
// %type <id> assignment

%%

/* descriptions of expected inputs     corresponding actions (in MIPS) */
program:
    BEGINN statements END               {if (yynerrs || yylexerrs) YYABORT; return -1;}
;

statements:
    statement
    | statements statement
;

statement:
        ID ASSIGNOP exp SEMICOLON           {updateSymbolTable($1,$3);}
    |   READ LPAREN IDs RPAREN SEMICOLON    {;}
    |   WRITE LPAREN exps RPAREN SEMICOLON  {;}
;

IDs: 
        ID      {symbolVal($1);}
    |   IDs COMMA ID {symbolVal($3);}
;

exps:
        exp         {printf("%d\n", $1);}
    |   exps COMMA exp  {printf("%d\n",$3);}
;

exp:
        primary        {$$ = $1;}
    |   exp PLUOP primary {$$ = $1 + $3;}
    |   exp MINUSOP primary {$$ = $1 - $3;}
;

primary:
        ID              {$$ = symbolVal($1);}
    |   INTLITERAL      {$$ = $1;}
    |   LPAREN exp RPAREN {$$ = $2;}
;


%%

int yylexerrs = 0;

void yyerror(char *s) {fprintf(stderr, "Syntax Error on line %s\n", s);}

/// main routine
int main(int argc, char** argv){
    //args
    if (argc > 2) {
        printf("wrong number of arguments.\n");
        return EXIT_FAILURE;
    } else if (argc == 2){
        char filename[50];
        sprintf(filename, "%s", argv[1]);
        // input test filename
        yyin = fopen(filename, "r");
    } else {
        yyin = stdin;
    }

    initSymbolTable();
    // call parser routines
    switch(yyparse()){
        case 0: 
            printf("compile success \n");
            break;
        case 1:
            printf("compile error\n");
            break;
        case 2:
            printf("not enough memory\n");
            break;
    }
    printf("\n error at %i\t Error string %i\n", yynerrs, yylexerrs);

    return 0;
}