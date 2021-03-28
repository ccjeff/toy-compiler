//
// Created by ccjeff on 2021/3/19.
//
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "fileIO.h"
#include "regexTree.h"
#include "DFA.h"
#include "scanner.h"

// all the base operations can be implemented with * and |. For example, the (+|-)?[digits]+
// is essentially just using (+|-|digits)[digits]*

std::vector<DFA*> getRules(){
    std::vector<DFA*> allRules;
    // define grammar rule
    auto digitTree = RegexTree("(+|-|1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*");
    auto idTree = RegexTree("(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)*(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|0|1|2|3|4|5|6|7|8|9|_)*");
    auto addTokenTree = RegexTree("+");
    auto minTokenTree = RegexTree("-");
    auto multiTokenTree = RegexTree("*");
    auto divTokenTree = RegexTree("/");
    auto andTokenTree = RegexTree("&");
    auto andandTokenTree = RegexTree("&&");
    auto equalTokenTree = RegexTree("=");
    auto semicolonTree = RegexTree(";");
    auto orTokenTree = RegexTree("|");
    auto ororTokenTree = RegexTree("||");
    auto commaTokenTree = RegexTree(",");
    auto lbracTokenTree = RegexTree("{");
    auto rbracTokenTree = RegexTree("}");
    auto lsquareTokenTree = RegexTree("[");
    auto rsquareTokenTree = RegexTree("]");
    auto lparenTokenTree = RegexTree("(");
    auto rparenTokenTree = RegexTree(")");
    auto notTokenTree = RegexTree("!");
    auto ltTokenTree = RegexTree("<");
    auto rtTokenTree = RegexTree(">");
    auto shiftLTokenTree = RegexTree("<<");
    auto shiftRTokenTree = RegexTree(">>");
    auto equTokenTree = RegexTree("==");
    auto neqTokenTree = RegexTree("!=");
    auto lessEquTokenTree = RegexTree("<=");
    auto greatEquTokenTree = RegexTree(">=");

    auto mainTokenTree = RegexTree("main");
    auto intTokenTree = RegexTree("int ");
    auto voidTokenTree = RegexTree("void");
    auto breakTokenTree = RegexTree("break");
    auto doTokenTree = RegexTree("do");
    auto elseTokenTree = RegexTree("else");
    auto ifTokenTree = RegexTree("if");
    auto whileTokenTree = RegexTree("while");
    auto returnTokenTree=  RegexTree("return");
    auto scanfTokenTree = RegexTree("scanf");
    auto printfTokenTree= RegexTree("printf");

    DFA* digitDFA = new DFA(digitTree);
    DFA* idDFA = new DFA(idTree);

    DFA* equalTokenDFA = new DFA(equalTokenTree);
    DFA* semicolonDFA = new DFA(semicolonTree);
    DFA* greatEquDFA = new DFA(greatEquTokenTree);
    DFA* lessEquDFA=  new DFA(lessEquTokenTree);
    DFA* neqTokenDFA = new DFA(neqTokenTree);
    DFA* equTokenDFA = new DFA(equTokenTree);
    DFA* shiftLDFA = new DFA(shiftLTokenTree);
    DFA* shiftRDFA = new DFA(shiftRTokenTree);
    DFA* ltDFA = new DFA(ltTokenTree);
    DFA* rtDFA = new DFA(rtTokenTree);
    DFA* notDFA = new DFA(notTokenTree);
    DFA* lparenDFA = new DFA(lparenTokenTree);
    DFA* rparenDFA = new DFA(rparenTokenTree);
    DFA* lsquareDFA = new DFA(lsquareTokenTree);
    DFA* rsquareDFA = new DFA(rsquareTokenTree);
    DFA* lbracDFA = new DFA(lbracTokenTree);
    DFA* rbracDFA = new DFA(rbracTokenTree);
    DFA* commaDFA = new DFA(commaTokenTree);
    DFA* orDFA = new DFA(orTokenTree);
    DFA* andandDFA = new DFA(andandTokenTree);
    DFA* andDFA = new DFA(andTokenTree);
    DFA* divDFA = new DFA(divTokenTree);
    DFA* multiDFA = new DFA(multiTokenTree);
    DFA* minDFA = new DFA(minTokenTree);
    DFA* addDFA = new DFA(addTokenTree);


    DFA* mainTokenDFA = new DFA(mainTokenTree);
    DFA* printfTokenDFA = new DFA(printfTokenTree);
    DFA* scanfTokenDFA = new DFA(scanfTokenTree);
    DFA* returnTokenDFA = new DFA(returnTokenTree);
    DFA* whileTokenDFA = new DFA(whileTokenTree);
    DFA* ifTokenDFA = new DFA(ifTokenTree);
    DFA* elseTokenDFA = new DFA(elseTokenTree);
    DFA* doTokenDFA = new DFA(doTokenTree);
    DFA* breakTokenDFA = new DFA(breakTokenTree);
    DFA* voidTokenDFA = new DFA(voidTokenTree);
    DFA* intTokenDFA = new DFA(intTokenTree);

    // RULES ARE PUT IN CORRECT ORDER TO AVOID 2 SAME MATCHES (e.g. main in MAIN and ID)
    allRules = {intTokenDFA, voidTokenDFA, breakTokenDFA, ifTokenDFA, elseTokenDFA, doTokenDFA,
                mainTokenDFA, printfTokenDFA,scanfTokenDFA, returnTokenDFA, whileTokenDFA,
                equalTokenDFA,semicolonDFA, addDFA, minDFA,multiDFA, divDFA, andDFA, andandDFA, orDFA, commaDFA,
                rbracDFA, lbracDFA, rsquareDFA, lsquareDFA,lparenDFA, rparenDFA,notDFA, rtDFA,ltDFA,shiftLDFA,shiftRDFA,
                equTokenDFA, neqTokenDFA, lessEquDFA, greatEquDFA,digitDFA,idDFA};
    return allRules;
}

std::string matchHelper(std::string* input,DFA* rule,FileIO* src){
    //returns the longest string reading from the start token matched by each DFA
    int counter = 1;
    bool firstMatch = false;
    while (counter != 0){
        counter = 0;
        bool isMatch = rule->simulateDFA(*input);
        counter += isMatch;
        if (isMatch) {firstMatch = true; input->push_back(src->getNextChar());}
    }
    if (firstMatch){
        input->pop_back();
        src->retractLex();
    }
    if (!firstMatch) return "";
    return *input;
}

int fetchToken(std::string* inputs, FileIO* src){
    // each round getting a string and return a type of token
    // first go find the matched dfa and read until it does not match
    auto allRules = getRules();
    std::string currentWord = "";
    int count = 0;
    int pos = -1;
    for (auto rule : allRules){
        // DEBUG use: create a .gv file describing the DFA
//        std::string filename = std::to_string(count);
//        filename += "_rule.gv";
//        rule->CreateDotFile(filename);
        auto res = matchHelper(inputs, rule, src);
        if (res.size() > currentWord.size()) {currentWord = res; pos = count;}
        count++;
    }
    std::cout << "TOKEN NAME: " << *inputs << std::endl;
    return pos;
}

std::string getIDName(int id){
    std::string returnName;
    switch (id){
        case 0:
            returnName = "INT";
            break;
        case 1:
            returnName = "VOID";
            break;
        case 2:
            returnName = "BREAK";
            break;
        case 3:
            returnName = "IF";
            break;
        case 4:
            returnName = "ELSE";
            break;
        case 5:
            returnName = "DO";
            break;
        case 6:
            returnName = "MAIN";
            break;
        case 7:
            returnName = "WRITE";
            break;
        case 8:
            returnName = "READ";
            break;
        case 9:
            returnName = "RETURN";
            break;
        case 10:
            returnName = "WHILE";
            break;
        case 11:
            returnName = "ASSIGN";
            break;
        case 12:
            returnName = "SEMI";
            break;
        case 13:
            returnName = "PLUS";
            break;
        case 14:
            returnName = "MINUS";
            break;
        case 15:
            returnName = "MUL_OP";
            break;
        case 16:
            returnName = "DIV_OP";
            break;
        case 17:
            returnName = "AND";
            break;
        case 18:
            returnName = "ANDAND";
            break;
        case 19:
            returnName = "OR";
            break;
        case 20:
            returnName = "COMMA";
            break;
        case 21:
            returnName = "RBRACE";
            break;
        case 22:
            returnName = "LBRACE";
            break;
        case 23:
            returnName = "RSQUARE";
            break;
        case 24:
            returnName = "LSQUARE";
            break;
        case 25:
            returnName = "LPAR";
            break;
        case 26:
            returnName = "RPAR";
            break;
        case 27:
            returnName = "NOT";
            break;
        case 28:
            returnName = "GT";
            break;
        case 29:
            returnName = "LT";
            break;
        case 30:
            returnName = "SHL_OP";
            break;
        case 31:
            returnName = "SHR_OP";
            break;
        case 32:
            returnName = "EQ";
            break;
        case 33:
            returnName = "NOTEQ";
            break;
        case 34:
            returnName = "LTEQ";
            break;
        case 35:
            returnName = "GTEQ";
            break;
        case 36:
            returnName = "INT_NUM";
            break;
        case 37:
            returnName = "ID";
            break;
        default:
            returnName = "";
            break;
    }
    return returnName;
}


int main(int argc, char* argv[]){
    if (argc != 2){
        std::cout << "should pass scanning filename to scanner" << std::endl;
        return -1;
    }
    else{
        std::string inputFilename = argv[1];
        FileIO* contentReader = new FileIO();
        contentReader->openFile(inputFilename);

        std::string startToken{contentReader->getNextChar()};
        while (contentReader->getLex() != contentReader->getBuffer().size()){
            int nextTokenID = fetchToken(&startToken, contentReader);
            std::cout << "TOKEN TYPE: " << getIDName(nextTokenID) << std::endl;
            // removing the leading spaces
            startToken = std::string(1,contentReader->getNextChar());
            while (startToken == " " || startToken == "\t" || startToken == "\n") startToken = std::string(1,contentReader->getNextChar());
        }

        auto allRules = getRules();
        for (auto rule : allRules){
            free(rule);
        }
        free(contentReader);
    }

    return 0;
}