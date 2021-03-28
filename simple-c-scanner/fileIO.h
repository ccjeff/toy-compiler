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
    int lexemeBegin = 0;

public:

    FileIO();
    int openFile(const std::string inputName);
    std::vector<char> getBuffer();

    char getNextChar();

    void increaseLex() {lexemeBegin++;}
    void retractLex() {lexemeBegin--;}
    int getLex() {return this->lexemeBegin;}

    ~FileIO();
};

#endif