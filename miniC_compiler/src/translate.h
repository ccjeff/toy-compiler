//
// Created by ccjeff on 2021/5/3.
//

#ifndef MINIC_COMPILER_TRANSLATE_H
#define MINIC_COMPILER_TRANSLATE_H

#include <map>
#include <string>
#include <iostream>
using std::string;

class Translator {
public:
    std::map<string, int> symbolTable;
    int last2Assign = -1;
    string lastTemp = "";
    string lastID = "";

    string getTemp(){
        static int max_temp = 0;
        string tempName = "Temp&" + std::to_string(max_temp);
        ++max_temp;
        symbolTable[tempName] = 0;
        std::cout << "Declare" << "," << tempName << "," << "Integer" << std::endl;
        return tempName;
    };

    void updateTable(string name, int val){
        if (symbolTable.find(name) == symbolTable.end()){
            symbolTable[name] = val;
            std::cout << "Declare" << "," << name << "," << "Integer" << std::endl;
        } else {
            symbolTable[name] = val;
        }
    }





};


#endif //MINIC_COMPILER_TRANSLATE_H
