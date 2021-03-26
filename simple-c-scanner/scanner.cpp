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
    auto digitTree = RegexTree("(+|-|1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*");
    auto idTree = RegexTree("(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)*(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|0|1|2|3|4|5|6|7|8|9|_)*");
    auto intTokenTree = RegexTree("int");
    auto mainTokenTree = RegexTree("main");
    auto equalTokenTree = RegexTree("=");

    DFA* intTokenDFA = new DFA(intTokenTree);
    DFA* digitDFA = new DFA(digitTree);
    DFA* idDFA = new DFA(idTree);
    DFA* equalTokenDFA = new DFA(equalTokenTree);
//    auto digitDFA = DFA(digitTree);
//    auto idDFA = DFA(idTree);
//    auto equalTokenDFA = DFA(equalTokenTree);
    allRules = {intTokenDFA, digitDFA, idDFA, equalTokenDFA};
    return allRules;
}

Token fetchToken(std::string_view inputs){

}


int main(int argc, char* argv[]){
    if (argc != 2){
        std::cout << "should pass scanning filename and regex filename to scanner" << std::endl;
        return -1;
    }
    else{
        std::string inputFilename = argv[1];
        FileIO contentReader;
        contentReader.openFile(inputFilename);
        auto raw = contentReader.getBuffer();
        int l,f = 0;

        auto rules = getRules();
        Token nextToken = fetchToken("TODO: content");

//        auto digitTree = RegexTree("(+|-|1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*");
//        auto idTree = RegexTree("(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)*(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|0|1|2|3|4|5|6|7|8|9|_)*");
//        auto intTokenTree = RegexTree("int");
//        auto mainTokenTree = RegexTree("main");
//        auto equalTokenTree = RegexTree("=");
//
//        auto idDFA = DFA(idTree);
//        bool isID = idDFA.simulateDFA("a_C");
//        std::cout << isID << std::endl;

//        std::vector<DFA> rules = {intTokenDFA,digitDFA,idDFA, equalTokenDFA};


//        bool isMain = intTokenDFA.simulateDFA("a9_c");


//        dfa.CreateDotFile("./debug.gv");
    }
    return 0;
}