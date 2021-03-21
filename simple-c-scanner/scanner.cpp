#include <iostream>
#include <string>

#include "fileIO.h"
#include "regexTree.h"
#include "DFA.h"

// A regular expression consists on letters, Kleene's closure (star '*')
// one more (plus '+'), zero or one (ques '?'), their alternatives
// and their sequences. Here is the grammar of it.
//
//     expression : concat ('|' concat)* ;
//     concat : piece* ;
//     piece : atom ('*' | '+' | '?')? ;
//     atom : letter | '\\' letter | '(' expression ')'
//
// To generate DFA from the regular expression, we need extended abstract
// syntax tree (AST). We extends the given AST concatenating with a SHARP
// node as the terminator.
//
//      (CAT <user's AST> SHARP)
//
//      e1|e2|e3  => (OR (OR <e1> <e2>) <e3>)
//      (e|)      => (OR <e> EPSILON)
//      e1 e2 e3  => (CAT (CAT <e1> <e2>) <e3>)
//      e*        => (STAR <e>)
//      e+        => (PLUS <e>)
//      e?        => (QUES <e>)
//      a         => (LETTER a)

int main(int argc, char* argv[]){
    if (argc != 2){
        std::cout << "should pass scanning filename and regex filename to scanner" << std::endl;
        return -1;
    }
    else{
        std::string inputFilename = argv[1];
        FileIO contentReader;
        contentReader.openFile(inputFilename);
        auto tree = regexTree("(a|b)*ab(a|b)*");
        auto dfa = DFA(tree);
        dfa.CreateDotFile("./debug.gv");
    }
    return 0;
}