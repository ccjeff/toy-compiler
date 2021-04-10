//
// Created by ccjeff on 2021/4/7.
//

#include <vector>
#include <fstream>
#include <iostream>
#include "parser.h"

void parser::parse(std::vector <Token> &tokens) {
    auto currentToken = tokens.begin();

}

void parser::readCFG(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    std::string lineStr;
    while (std::getline(file, lineStr)) {
        lineStr.push_back('|');
        lineStr.erase(remove_if(lineStr.begin(), lineStr.end(), isspace), lineStr.end());
        size_t posColon = lineStr.find(':');
        std::string left = lineStr.substr(0, posColon);
        this->ordered_keys.push_back(left);
        lineStr.erase(0,posColon+1);
        std::vector<std::string> rules;
        size_t posOr = 0;
        while ((posOr = lineStr.find('|')) != std::string::npos) {
            rules.push_back(lineStr.substr(0,posOr));
            lineStr.erase(0, posOr + 1);
        }
        this->productions[left] = rules;
        for (auto s : rules){
            std::cout << s << std::endl;
        }
    }
    file.close();
}

void parser::removeLeftRecursion() {
    size_t pSize = this->productions.size();
    for (size_t i = 0; i < pSize; i++){
        auto nonTerm = this->ordered_keys[i];
        auto prods = this->productions[nonTerm];
        for (size_t j = 0; j < i-1; j++){
            auto prod = prods[j];

        }
    }
}

void parser::removeImmediateRecursion(std::string left, std::vector<std::string> prods) {
    std::string newLeft = left + "\'";
    std::vector<std::string> newProds;
    std::vector<std::string> oldProds;
    bool isImmediate = false;
    for (auto prod : prods){
        int tag = 0;
        if (prod.find(left) == std::string::npos) oldProds.emplace_back(prod);
        else isImmediate = true;
        size_t index = 0;
        while (true){
            index = prod.find(left, index);
            if (index == std::string::npos) break;
            tag++;
            prod.erase(index, left.size());
            prod += newLeft;
            index += newLeft.size();
        }
        if (tag!=0) newProds.emplace_back(prod);
    }
    if (isImmediate){
        std::vector<std::string>::iterator old;
        for (old = oldProds.begin();old != oldProds.end();old++){
            old->append(newLeft);
        }
        this->productions[left] = oldProds;
        this->productions.emplace(newLeft, newProds);
        this->ordered_keys.emplace_back(newLeft);
    }

}

void parser::debug() {
    size_t pSize = this->productions.size();
    for (size_t i = 0; i < pSize; i++){
        auto nonTerm = this->ordered_keys[i];
        auto prods = this->productions[nonTerm];
        removeImmediateRecursion(nonTerm,prods);
    }
    printProds();
}

void parser::printProds() {
    std::cout << "the keys & vals are: " << std::endl;
    for (const auto& a : this->productions){
        std::cout << a.first << "----";
        for (const auto& v : a.second){
            std::cout << v;
        }
    }
    std::cout << "\n" << std::endl;

}
