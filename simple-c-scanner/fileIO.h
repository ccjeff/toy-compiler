//
// Created by ccjeff on 2021/3/17.
//
#ifndef FILE_IO_H
#define FILE_IO_H

#include <vector>
#include <string>
class FileIO
{
private:
    /* data */
    std::vector<char> content;
    std::vector<char> buffer;
    int lexemeBegin = 0;
    int forward = 0;

public:

    FileIO();
    int openFile(const std::string inputName);
    std::vector<char> getBuffer();

    char getNextChar();

    void increaseLex() {lexemeBegin++;}
    void retractLex() {lexemeBegin--;}
    void increaseLexN(int n) {lexemeBegin += n;}
    void retractLexN(int n) {lexemeBegin -= n;}

    int getLex() {return this->lexemeBegin;}
    int getForward() {return this->forward;}

    ~FileIO();
};

#endif