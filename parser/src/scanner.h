#pragma once
enum Token {
    INT, MAIN, VOID, BREAK, DO, ELSE, IF,
    WHILE, RETURN, READ, WRITE, LBRACE,
    RBRACE, LSQUARE, RSQUARE, LPAR, RPAR, SEMI,
    PLUS, MINUS, MUL_OP, DIV_OP, AND_OP, OR_OP, NOT_OP,
    ASSIGN, LT, GT, SHL_OP, SHR_OP, EQ, NOTEQ, LTEQ, GTEQ,
    ANDAND, OROR, COMMA
};


extern int token;
extern int line;

int scannerCalls(int argc, char* argv[]);