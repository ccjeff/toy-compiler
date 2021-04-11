//
// Created by ccjeff on 2021/4/7.
//
#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include "scanner.h"


class parser {
private:
    std::unordered_map<std::string, std::vector<std::string>> productions;
    std::vector<std::string> ordered_keys;
    void printProds();
    std::string longestCommonPrefix(std::vector<std::string>& strs);

public:
    void parse(std::vector<Token> &tokens);
    void readCFG(std::string filename);
    void removeLeftRecursion();
    void removeImmediateRecursion(std::string left, std::vector<std::string> prods);
    void removeCommonLeft();

};


#endif //PARSER_PARSER_H
