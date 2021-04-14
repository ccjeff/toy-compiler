#ifndef SIMPLE_COMPILER_FOR_MIPS_SCANNER_H
#define SIMPLE_COMPILER_FOR_MIPS_SCANNER_H


#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <sstream>
#include <utility>

using std::string;
using std::unordered_map;

enum TokenType {
    INVALID,
    ID,
    INT,
    MAIN,
    VOID,
    BREAK, DO, IF, ELSE, WHILE,
    RETURN,
    READ, WRITE,
    LBRACE, RBRACE,
    LSQUARE, RSQUARE,
    LPAR, RPAR,
    SEMI, PLUS, MINUS, MUL_OP, DIV_OP, AND_OP, OR_OP, NOT_OP,
    ASSIGN, LT, GT, SHL_OP, SHR_OP, EQ, NOTEQ, LTEQ, GTEQ,
    ANDAND, OROR, COMMA,
    INT_NUM
};

const char *token2string(TokenType tokenType);

class Scanner {
private:
    const std::regex REGEX_INT_NUM;
    const std::regex REGEX_ID;
    const std::regex REGEX_SPECIAL;

    unordered_map<string, TokenType> special2token, keyword2token;
    string buffer;
    size_t currentTokenOff, currentTokenLen;  // current position pointer of the scanner
    TokenType currentTokenType;

public:
    /* construct scanner from the buffer */
    explicit Scanner(string buffer);

    /* construct the scanner from file*/
    static Scanner fromFile(std::ifstream &fin);

    /* True if there is a next token, false otherwise*/
    bool nextToken();

    /* Scanner containing a token to read*/
    bool hasToken();

    TokenType getTokenType() const;

    string getTokenValue() const;
};


#endif //SIMPLE_COMPILER_FOR_MIPS_SCANNER_H
