//
// Created by ccjeff on 2021/3/17.
//
#include <iostream>
#include <fstream>
#include <vector>
#include "fileIO.h"


FileIO::FileIO() = default;
FileIO::~FileIO() = default;

int FileIO::openFile(const std::string inputName) {
    std::fstream fin(inputName, std::fstream::in);
    char ch;
    while (fin >> std::noskipws >> ch) {
        content.emplace_back(ch);
    }

    return 0;
}

std::vector<char> FileIO::getBuffer() {
    return this->content;
}

char FileIO::getNextChar() {
    if (lexemeBegin >= content.size()) return '\0';
    char c = content[lexemeBegin];
    this->increaseLex();
    return c;
}

