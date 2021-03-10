%{

#include "translate.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"


void yyerror(char* s);
extern int yynerrs;
extern int yylexerrs;
extern FILE* yyin;

int yylex();
%}

%union {
    int num;
    char* id;
    expr_rec expr_record;
}         /* Yacc definitions for micro lang*/

%token <num> INTLITERAL
%token <id> ID

%token BEGINN
%token END
%token READ
%token WRITE
%token LPAREN
%token RPAREN
%token SEMICOLON
%token COMMA
%token ASSIGNOP
%token PLUOP
%token MINUSOP
%token SCANEOF

%left 
PLUOP MINUSOP COMMA
%right ASSIGNOP

%type <expr_record> exp primary


%%

/* descriptions of expected inputs     corresponding actions */
program:
    BEGINN statements END SCANEOF              {finalize(); if (yynerrs || yylexerrs) YYABORT; return -1;}
;

statements:
    statement
    | statements statement
;

statement:
        ID ASSIGNOP exp SEMICOLON           {
                                            assignEntry($1);
                                            expr_rec* t = (expr_rec*)malloc(sizeof(expr_rec));
                                            // t->val = $1;
                                            t->kind = IDEXPR;
                                            sprintf(t->name,"%s", $1);
                                            assignment(*t,$3);}
    |   READ LPAREN IDs RPAREN SEMICOLON  
    |   WRITE LPAREN exps RPAREN SEMICOLON
;

IDs: 
        ID           {
                        assignEntry($1);
                        expr_rec temp;
                        temp.kind = IDEXPR;
                        strcpy(temp.name, $1);
                        readID(temp);

                    }
    |   IDs COMMA ID {
                        assignEntry($3);
                        expr_rec temp;
                        temp.kind = IDEXPR;
                        strcpy(temp.name, $3);
                        readID(temp);
                    }
;

exps:
        exp             {assignEntry($1.name);writeID($1);}
    |   exps COMMA exp  {assignEntry($3.name);writeID($3);}
;

exp:
        primary             {$$ = $1;}
    |   exp PLUOP primary   {
                                $$ = gen_infix($1,"Add",$3);
                            }
    |   exp MINUSOP primary {$$ = gen_infix($1,"Min",$3);}
;

primary:
        ID                  {
                                expr_rec* t = (expr_rec*)malloc(sizeof(expr_rec));
                                t->kind = IDEXPR;
                                sprintf(t->name, "%s",$1);
                                $$ = *t;
                            }
    |   INTLITERAL          {
                                
                                expr_rec* t = (expr_rec*)malloc(sizeof(expr_rec));
                                t->kind = LITERALEXPR;
                                t->val = $1;
                                sprintf(t->name, "INTEGER");   // fake name
                                $$ = *t;
                            }
    |   LPAREN exp RPAREN   {$$ = $2;}
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
        char filename[MAX_FILENAME];
        sprintf(filename, "%s", argv[1]);
        // printf("the filename is: %s \n", argv[1]);
        // input test filename
        yyin = fopen(filename, "r");
        // printf("open succeeded \n");
    } else {
        yyin = stdin;
    }
    printf("/////START COMPILING/////\n");
    initSymbolTable();
    // printf("symbolTable inited\n");
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