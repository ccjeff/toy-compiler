//
// Created by ccjeff on 2021/4/7.
//

#include <vector>
#include <fstream>
#include <iostream>
#include "parser.h"

void parser::parse(std::vector <Token> &tokens) {
//    removeLeftRecursion();
    removeCommonLeft();
    printProds();


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
        auto prods = this->productions[nonTerm];  // all productions of A_i
        for (size_t j = 0; j < i; j++){
            auto nonTermJ = this->ordered_keys[j];  //key of A_j
            auto jProds = this->productions[nonTermJ]; // all productions of A_j
            std::vector<std::string>::iterator p;
            std::vector<std::string> newProds;
            for (p = prods.begin();p != prods.end();p++){  // sub for all A_i --> A_j gamma
                size_t index = 0;
                if (p->find(nonTermJ) == std::string::npos) newProds.emplace_back(*p);
                if (p->find(nonTermJ) != std::string::npos){
                    //should sub for all
                    for (const auto& pj : jProds){
                        std::string prodcpy = *p;  // copy for the original word to sub
                        while (true){
                            index = prodcpy.find(nonTermJ);
                            if (index == std::string::npos) break;
                            prodcpy.replace(index, nonTermJ.size(), pj);
                            newProds.emplace_back(prodcpy);
                        }
                    }
                }
            }
            this->productions[nonTerm] = newProds;
        }
        removeImmediateRecursion(nonTerm,this->productions[nonTerm]);
    }
    printProds();
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
            if (*old != "@") old->append(newLeft);
            else (*old = newLeft);
        }
        newProds.emplace_back("@");
        this->productions[left] = oldProds;
        this->productions.emplace(newLeft, newProds);
        this->ordered_keys.emplace_back(newLeft);
    }

}


void parser::printProds() {
    std::cout << "the keys & vals are: " << std::endl;
    for (const auto& a : this->productions){
        std::cout << a.first << "----";
        for (const auto& v : a.second){
            std::cout << v;
            std::cout << " ";
        }
        std::cout << "\n" << std::endl;
    }
    std::cout << "in the ordered keys: " << std::endl;
    for (auto i : this->ordered_keys){
        std::cout << i << " ";
    }
    std::cout << "\n" << std::endl;
}

std::string parser::longestCommonPrefix(std::vector<std::string> &strs) {
    if (strs.size() == 0) return "";
    auto longest = strs[0].size();

    for (int i = 1; i < strs.size(); i++) {
        char* word_a = strs[0].data();
        char* word_b = strs[i].data();
        while (*word_a != 0 && *word_b != 0 && *word_a == *word_b) {
            ++word_a;
            ++word_b;
        }
        // Update longest element.
        if (word_a - strs[0].data() < longest) {
            longest = word_a - strs[0].data();
        }
    }

    *(strs[0].data() + longest) = '\0';

    return strs[0].data();
}


void parser::removeCommonLeft() {
    for (auto key : this->ordered_keys){
        // get the longest prefix for the key
        std::vector<std::string> prods;
        prods = this->productions[key];
        auto longestPref = longestCommonPrefix(prods);
        std::vector<std::string> newProds;
        std::vector<std::string>::iterator old;
        for (old = prods.begin();old != prods.end();old++){
            if ((*old)[0] != longestPref[0]){
                continue;
            } else {
//                 add to new
                std::string oldcpy = *old;
                oldcpy.erase(oldcpy.begin(), oldcpy.begin()+longestPref.size());
                newProds.emplace_back(oldcpy);
//                remove this.
//                prods.erase(old);
            }
        }
        std::string newKey = key + "`";
//        prods.insert(prods.begin(), longestPref + newKey);
//        this->productions.emplace(newKey, newProds);
    }
    printProds();
}



