%{
    // #include "scanner.h"
    #include "y.tab.h"
    #include "stdio.h"
    #include "string.h"

    void yyerror(char* s);
    int yylex();
%}

%%

:=              return ASSIGNOP;
,               return COMMA;
\+               return PLUOP;
--.*\n                  ;
-               return MINUSOP;
;               return SEMICOLON;
\(               return LPAREN;
\)               return RPAREN;
\$               return SCANEOF;

"BEGIN"         return BEGINN;
"END"           return END;
"READ"          return READ;
"WRITE"         return WRITE;

[a-zA-Z][-_a-zA-Z0-9]*	return ID;
[1-9][0-9]*				return INTLITERAL;
[ \t\n]                 ;
.					printf("unexpected character\n");

%%

int yywrap(void){
    return 1;
}