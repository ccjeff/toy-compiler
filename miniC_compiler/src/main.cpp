#include <memory>
#include <iostream>

#include "scanner.h"
#include "parser.h"
#include "translate.h"

int main(int argc, char *argv[]) {
    std::ifstream fin(argc > 1 ? argv[1] : "./testfiles/test_simple.in");
    Scanner scanner = Scanner::fromFile(fin);
    Translator translator;

    fin.close();

    try {
        scanner.nextToken();
        shared_ptr<Program> program = Program::parse(scanner, translator);

//        std::vector<string> rows;
//        program->fillPrettyStrings(rows, 2, 0);

//        for (auto &row : rows) {
//            std::cout << row << std::endl;
//        }
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
