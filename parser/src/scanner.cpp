//
// Created by ccjeff on 2021/3/19.
//
#include "scanner.h"

const char *token2string(TokenType tokenType) {
    switch (tokenType) {
        case INVALID:
            return "INVALID";
        case ID:
            return "ID";
        case INT:
            return "int";
        case MAIN:
            return "main";
        case VOID:
            return "void";
        case BREAK:
            return "break";
        case DO:
            return "do";
        case IF:
            return "if";
        case ELSE:
            return "else";
        case WHILE:
            return "while";
        case RETURN:
            return "return";
        case READ:
            return "scanf";
        case WRITE:
            return "printf";
        case LBRACE:
            return "{";
        case RBRACE:
            return "}";
        case LSQUARE:
            return "[";
        case RSQUARE:
            return "]";
        case LPAR:
            return "(";
        case RPAR:
            return ")";
        case SEMI:
            return ";";
        case PLUS:
            return "+";
        case MINUS:
            return "-";
        case MUL_OP:
            return "*";
        case DIV_OP:
            return "/";
        case AND_OP:
            return "&";
        case OR_OP:
            return "|";
        case NOT_OP:
            return "!";
        case ASSIGN:
            return "=";
        case LT:
            return "<";
        case GT:
            return ">";
        case SHL_OP:
            return "<<";
        case SHR_OP:
            return ">>";
        case EQ:
            return "==";
        case NOTEQ:
            return "!=";
        case LTEQ:
            return "<=";
        case GTEQ:
            return ">=";
        case ANDAND:
            return "&&";
        case OROR:
            return "||";
        case COMMA:
            return ",";
        case INT_NUM:
            return "NUMBER";
        default:
            return "INVALID";
    }
}

Scanner::Scanner(string buffer)
        : REGEX_INT_NUM("[+|-]?[0-9]+"),
          REGEX_ID("[a-zA-Z]+[0-9|a-zA-Z|_]*"),
          REGEX_SPECIAL(R"(&&|\|\||<=|>=|==|<<|>>|!=|&|\||<|>|=|\(|\)|\{|\}|\.|,|\+|-|\*|/|\[|\]|!|;)"),
          buffer(std::move(buffer)),
          currentTokenOff(0), currentTokenLen(0),
          currentTokenType(INVALID) {
    // std::cout << "{" << this->buffer << "}" << std::endl;
    special2token["{"] = LBRACE;
    special2token["}"] = RBRACE;
    special2token["["] = LSQUARE;
    special2token["]"] = RSQUARE;
    special2token["("] = LPAR;
    special2token[")"] = RPAR;
    special2token[";"] = SEMI;
    special2token["+"] = PLUS;
    special2token["-"] = MINUS;
    special2token["*"] = MUL_OP;
    special2token["/"] = DIV_OP;
    special2token["&"] = AND_OP;
    special2token["|"] = OR_OP;
    special2token["!"] = NOT_OP;
    special2token["="] = ASSIGN;
    special2token["<"] = LT;
    special2token[">"] = GT;
    special2token["<<"] = SHL_OP;
    special2token[">>"] = SHR_OP;
    special2token["=="] = EQ;
    special2token["!="] = NOTEQ;
    special2token["<="] = LTEQ;
    special2token[">="] = GTEQ;
    special2token["&&"] = ANDAND;
    special2token["||"] = OROR;
    special2token[","] = COMMA;

    keyword2token["int"] = INT;
    keyword2token["main"] = MAIN;
    keyword2token["void"] = VOID;
    keyword2token["break"] = BREAK;
    keyword2token["do"] = DO;
    keyword2token["else"] = ELSE;
    keyword2token["if"] = IF;
    keyword2token["while"] = WHILE;
    keyword2token["return"] = RETURN;
    keyword2token["scanf"] = READ;
    keyword2token["printf"] = WRITE;
}

Scanner Scanner::fromFile(std::ifstream &fin) {
    std::stringstream buffer;
    buffer << fin.rdbuf();
    return Scanner(buffer.str());
}

/* True if there is a next token, false otherwise*/
bool Scanner::nextToken() {
    currentTokenOff += currentTokenLen;
    currentTokenLen = 0;
    // !EOF
    while (currentTokenOff < buffer.size()) {
        char ch = buffer[currentTokenOff];
        if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
            ++currentTokenOff;
        } else {
            break;
        }
    }

    if (currentTokenOff >= buffer.size()) {
        return false;
    }
    // matching token patterns
    std::smatch results;
    while (currentTokenOff + currentTokenLen + 1 <= buffer.size()) {
        auto l = buffer.cbegin() + currentTokenOff;
        auto r = l + currentTokenLen + 1;
        if (std::regex_match(l, r, results, REGEX_INT_NUM) ||
            std::regex_match(l, r, results, REGEX_SPECIAL) ||
            std::regex_match(l, r, results, REGEX_ID)) {
            ++currentTokenLen;
            continue;
        } else {
            break;
        }
    }

    if (currentTokenLen == 0) {
        // Unexpected Token Type
        currentTokenType = INVALID;
        currentTokenLen = 1;
        return currentTokenOff < buffer.size();
    }

    auto l = buffer.cbegin() + currentTokenOff;
    auto r = l + currentTokenLen;
    if (std::regex_match(l, r, results, REGEX_INT_NUM)) {
        currentTokenType = INT_NUM;
    } else if (std::regex_match(l, r, results, REGEX_SPECIAL)) {
        currentTokenType = special2token[buffer.substr(currentTokenOff, currentTokenLen)];
    } else if (std::regex_match(l, r, results, REGEX_ID)) {
        auto it = keyword2token.find(buffer.substr(currentTokenOff, currentTokenLen));
        if (it != keyword2token.end()) {
            currentTokenType = keyword2token[it->first];
        } else {
            currentTokenType = ID;
        }
    }
    return true;
}

bool Scanner::hasToken() {
    return !(currentTokenOff >= buffer.size() ||
             currentTokenOff + currentTokenLen > buffer.size());
}

TokenType Scanner::getTokenType() const {
    return currentTokenType;
}

string Scanner::getTokenValue() const {
    if (currentTokenOff + currentTokenLen > buffer.size() || currentTokenLen == 0) {
        return "";
    } else {
        return buffer.substr(currentTokenOff, currentTokenLen);
    }
}
