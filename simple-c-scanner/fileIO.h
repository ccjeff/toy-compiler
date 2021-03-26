#ifndef FILE_IO_H
#define FILE_IO_H

#include <vector>
#include <string>
class FileIO
{
private:
    /* data */
    std::vector<std::string> content;
    std::vector<std::string> buffer;
    int lexemeBegin = 0;
    int forward = 0;

public:

    FileIO();
    int openFile(const std::string inputName);
    std::vector<std::string> getBuffer();



    void clearBuffer() {buffer.clear();}
    void increaseLex() {lexemeBegin++;}
    void increaseForward() {forward++;}
    int getLex() {return this->lexemeBegin;}
    int getForward() {return this->forward;}

    ~FileIO();
};

#endif